-- DCF.Sim.Channel — simulated 900 MHz §15.249 RF channel
-- DeMoD LLC
--
-- Models the impairments a real 900 MHz ISM link would produce:
--   - Packet loss (Bernoulli, configurable drop rate)
--   - Bit error rate applied post-encode (AWGN proxy)
--   - Fixed propagation delay
--
-- BUG FIXES vs previous version:
--   1. transmit used flushTQueue+re-enqueue to measure depth — a race.
--      Replaced with TBQueue (bounded queue) for correct back-pressure.
--   2. receive used readTQueue inside atomically with alive check — STM retry
--      semantics caused it to block forever after closeChannel if the queue
--      was empty. Fixed with orElse: try the queue, else check alive.

module DCF.Sim.Channel
  ( ChannelConfig (..)
  , defaultChannelConfig
  , stageConfig
  , studioConfig
  , noisyConfig
  , RfChannel
  , newChannel
  , transmit
  , receive
  , closeChannel
  ) where

import Control.Concurrent     (threadDelay, forkIO)
import Control.Concurrent.STM
import Control.Monad          (when, forever, void)
import Data.ByteString        (ByteString)
import qualified Data.ByteString as BS
import Data.Bits              (xor, shiftL)
import Data.Word              (Word8)
import System.Random          (randomRIO)

-- ── Config ─────────────────────────────────────────────────────────────────────

data ChannelConfig = ChannelConfig
  { chanDropRate     :: Double  -- ^ Bernoulli packet loss prob (0.0-1.0)
  , chanBitErrorRate :: Double  -- ^ Per-bit error probability (0.0-1.0)
  , chanDelayUs      :: Int     -- ^ Fixed propagation delay (µs)
  , chanQueueDepth   :: Int     -- ^ Max frames in flight (TBQueue bound)
  } deriving (Show, Eq)

defaultChannelConfig :: ChannelConfig
defaultChannelConfig = ChannelConfig
  { chanDropRate     = 0.01
  , chanBitErrorRate = 1e-5
  , chanDelayUs      = 500
  , chanQueueDepth   = 64
  }

stageConfig :: ChannelConfig
stageConfig = defaultChannelConfig
  { chanDropRate     = 0.005
  , chanBitErrorRate = 5e-6
  , chanDelayUs      = 200
  }

studioConfig :: ChannelConfig
studioConfig = defaultChannelConfig
  { chanDropRate     = 0.001
  , chanBitErrorRate = 1e-6
  , chanDelayUs      = 50
  }

noisyConfig :: ChannelConfig
noisyConfig = defaultChannelConfig
  { chanDropRate     = 0.08
  , chanBitErrorRate = 5e-4
  , chanDelayUs      = 1500
  }

-- ── Channel ───────────────────────────────────────────────────────────────────

data RfChannel = RfChannel
  { _txQ    :: TBQueue ByteString  -- bounded: provides back-pressure
  , _rxQ    :: TBQueue ByteString
  , _cfg    :: ChannelConfig
  , _alive  :: TVar Bool
  }

newChannel :: ChannelConfig -> IO RfChannel
newChannel cfg = do
  txQ   <- newTBQueueIO (fromIntegral $ chanQueueDepth cfg)
  rxQ   <- newTBQueueIO (fromIntegral $ chanQueueDepth cfg)
  alive <- newTVarIO True
  let ch = RfChannel txQ rxQ cfg alive
  void $ forkIO (channelThread ch)
  return ch

closeChannel :: RfChannel -> IO ()
closeChannel ch = atomically $ writeTVar (_alive ch) False

-- ── TX / RX ───────────────────────────────────────────────────────────────────

-- | Non-blocking transmit. Silently drops if the TX queue is full
--   (models back-pressure / transmitter overflow).
transmit :: RfChannel -> ByteString -> IO ()
transmit ch bs = atomically $ do
  full <- isFullTBQueue (_txQ ch)
  when (not full) $ writeTBQueue (_txQ ch) bs

-- | Block until a frame arrives. Returns Nothing when the channel is closed
--   AND the RX queue has been fully drained.
--
--   STM orElse semantics:
--     - Try to read from rxQ (succeeds immediately if a frame is ready)
--     - If rxQ is empty, retry — but orElse catches the retry and checks
--       alive. If alive=False and rxQ empty, return Nothing.
--     - This correctly unblocks after closeChannel without deadlock.
receive :: RfChannel -> IO (Maybe ByteString)
receive ch = atomically $
  (Just <$> readTBQueue (_rxQ ch))
  `orElse`
  do alive <- readTVar (_alive ch)
     if alive
       then retry          -- channel live, queue temporarily empty — wait
       else return Nothing -- channel closed and queue drained

-- ── Impairment thread ─────────────────────────────────────────────────────────

channelThread :: RfChannel -> IO ()
channelThread ch = forever $ do
  -- Block until alive or a frame arrives
  mFrame <- atomically $ do
    alive <- readTVar (_alive ch)
    if not alive
      then return Nothing
      else Just <$> readTBQueue (_txQ ch)
  case mFrame of
    Nothing    -> return ()
    Just frame -> do
      drop' <- shouldDrop (chanDropRate (_cfg ch))
      if drop'
        then return ()
        else do
          frame' <- applyBitErrors (chanBitErrorRate (_cfg ch)) frame
          threadDelay (chanDelayUs (_cfg ch))
          atomically $ do
            full <- isFullTBQueue (_rxQ ch)
            when (not full) $ writeTBQueue (_rxQ ch) frame'

shouldDrop :: Double -> IO Bool
shouldDrop rate = (< rate) <$> randomRIO (0.0, 1.0)

applyBitErrors :: Double -> ByteString -> IO ByteString
applyBitErrors ber bs =
  BS.pack <$> mapM (applyBitErrorsByte ber) (BS.unpack bs)

applyBitErrorsByte :: Double -> Word8 -> IO Word8
applyBitErrorsByte ber b = do
  masks <- mapM (\i -> do
    flip' <- (< ber) <$> randomRIO (0.0, 1.0)
    return $ if flip' then ((1 :: Word8) `shiftL` i) else 0)
    [0..7 :: Int]
  return $ foldl xor b masks
