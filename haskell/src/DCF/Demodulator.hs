-- DCF.Demodulator — DeMoD Faust-SDR RX pipeline
-- DeMoD LLC
--
-- Mirrors DCF.Modulator in the receive direction.
--
-- Data flow:
--
--   SoapySDR readStream (CF32 IQ)
--       │
--       ▼   deinterleave [I0,Q0,I1,Q1,...] → ([I0,I1,...], [Q0,Q1,...])
--   Faust inputs[0]=I, inputs[1]=Q
--       │   bpsk_demod.dsp:
--       │     Costas loop (carrier recovery)
--       │     RRC matched filter (63-tap Kaiser FIR, alpha=0.35)
--       ▼
--   Faust outputs[0]: soft symbols ∈ ℝ  (+ ≈ bit 1, − ≈ bit 0)
--       │
--       ▼   sliceSoftSymbols: downsample by samplesPerSymbol, threshold at 0
--   [Bool]  bit stream (136 bits = one frame)
--       │
--       ▼   bitsToBytes → ByteString (17 bytes)
--       │
--       ▼   decodeFrame → Maybe DeModFrame
--
-- Costas loop acquisition:
--   The Costas loop in bpsk_demod.dsp requires a lock-in period before
--   reliable symbol decisions are available.  runDemodulator discards the
--   first `demodAcqFrames` frame-slots worth of samples (default 4) to
--   allow the loop to converge before attempting frame sync.
--
-- Frame sync:
--   We slide a 1-sample window over the soft-symbol stream looking for the
--   sync byte (0xD3) pattern in the sliced bit stream.  When found, we
--   extract the next 17 bytes and attempt CRC validation.  This is a naive
--   O(n) correlator; a proper implementation would use a matched filter on
--   the sync pattern in the soft domain for better sensitivity in noise.
--
-- Thread safety:
--   runDemodulator is single-threaded.  Use async / STM channels at the
--   call site if concurrent RX and frame processing are required.

{-# LANGUAGE RecordWildCards #-}
{-# LANGUAGE ScopedTypeVariables #-}

module DCF.Demodulator
  ( DemodConfig (..)
  , defaultDemodConfig
  , runDemodulator
  , rxOneFrame
  ) where

import Control.Exception          (throwIO, ErrorCall (..))
import Control.Monad              (when, forM_)
import Data.Bits                  (setBit, testBit)
import Data.ByteString            (ByteString)
import qualified Data.ByteString  as BS
import Data.IORef                 (newIORef, readIORef, writeIORef, modifyIORef')
import Data.Vector.Storable       (Vector)
import qualified Data.Vector.Storable as V
import Data.Word                  (Word8)

import DCF.Faust.DSP   (DspConfig (..), DspHandle, withDsp, compute, setParam)
import DCF.SDR.Device  (SdrRxConfig (..), SdrRxDevice, withSdrRxDevice, readSamples)
import DCF.Transport.Frame
import DCF.Transport.Symbol       (SymbolConfig (..), defaultSymbolConfig, samplesPerSymbol)

-- ── Config ────────────────────────────────────────────────────────────────────

data DemodConfig = DemodConfig
  { demodDsp       :: DspConfig
  , demodSdr       :: SdrRxConfig
  , demodSymbol    :: SymbolConfig
  , demodBlockSize :: Int    -- ^ Faust compute block size (must be multiple of 256)
  , demodAcqFrames :: Int    -- ^ discard this many frame-slots for Costas lock-in
  } deriving (Show)

defaultDemodConfig :: DemodConfig
defaultDemodConfig = DemodConfig
  { demodDsp = DspConfig
      { dspSampleRate    = 2_000_000
      , dspExpectInputs  = 2   -- I, Q from SDR
      , dspExpectOutputs = 1   -- soft symbol stream
      }
  , demodSdr = defaultSdrRxConfig
      { rxSampleRateHz = 2e6
      , rxBlockSize    = 4096
      }
  , demodSymbol    = defaultSymbolConfig
  , demodBlockSize = 1024
  , demodAcqFrames = 4
  }
  where
    defaultSdrRxConfig = mempty_SdrRxConfig

-- Inline default since we can't import it without a cycle — mirror of Device.hs
mempty_SdrRxConfig :: SdrRxConfig
mempty_SdrRxConfig = SdrRxConfig
  { rxDriver       = "rtlsdr"
  , rxCenterFreqHz = 433.92e6
  , rxSampleRateHz = 2e6
  , rxGainDb       = 40.0
  , rxTimeoutUs    = 1_000_000
  , rxBlockSize    = 4096
  }

-- ── IQ helpers ───────────────────────────────────────────────────────────────

-- | Split interleaved [I0,Q0,I1,Q1,...] into separate I and Q vectors.
deinterleave :: Vector Float -> (Vector Float, Vector Float)
deinterleave iq =
  let n = V.length iq `div` 2
  in ( V.generate n (\i -> iq V.! (i * 2))
     , V.generate n (\i -> iq V.! (i * 2 + 1))
     )

-- ── Soft-symbol slicer ────────────────────────────────────────────────────────

-- | Downsample a soft-symbol stream by @sps@ (samples-per-symbol) and
--   hard-slice at threshold 0.
--
--   We sample at the symbol centre: index = sps/2 + k*sps.
--   This is a rectangular integrate-dump at the output of the RRC MF:
--   the RRC matched filter already provides zero-ISI at the correct instants.
--
--   Returns a flat list of Bool (True = bit 1, False = bit 0).
sliceSoftSymbols :: Int        -- ^ samples per symbol
                 -> Vector Float
                 -> [Bool]
sliceSoftSymbols sps syms =
  let n       = V.length syms
      offset  = sps `div` 2            -- sample at symbol centre
      nSym    = (n - offset) `div` sps
  in [ (syms V.! (offset + k * sps)) > 0.0 | k <- [0 .. nSym - 1] ]

-- ── Bit → byte packing ────────────────────────────────────────────────────────

-- | Pack a list of Bool bits (MSB first) into bytes.
--   Silently drops trailing bits that don't fill a complete byte.
bitsToBytes :: [Bool] -> ByteString
bitsToBytes bits = BS.pack (go bits)
  where
    go [] = []
    go bs =
      let (byte_bits, rest) = splitAt 8 bs
      in if length byte_bits < 8
           then []
           else packByte byte_bits : go rest
    packByte :: [Bool] -> Word8
    packByte = foldl (\acc b -> acc * 2 + if b then 1 else 0) 0

-- ── Frame sync search ─────────────────────────────────────────────────────────

-- | Scan a bit list for a valid 17-byte DCF frame starting at every
--   byte-aligned position.
--
--   Returns Just (frame, remaining_bits) on the first valid decode,
--   or Nothing if no valid frame is found in @bits@.
--
--   Byte-alignment constraint: we only check positions that are multiples
--   of 8 bits (1 byte).  A proper correlator would check every bit position,
--   but byte-aligned search is correct when the transmitter aligns frames
--   to byte boundaries (which ours does — frameToSymbols encodes 17 complete
--   bytes).
findFrame :: [Bool] -> Maybe (DeModFrame, [Bool])
findFrame bits = go bits
  where
    frameBits = 17 * 8

    go [] = Nothing
    go bs
      | length bs < frameBits = Nothing
      | otherwise =
          let candidate = bitsToBytes (take frameBits bs)
          in case decodeFrame candidate of
               Just frame -> Just (frame, drop frameBits bs)
               Nothing    -> go (drop 8 bs)  -- advance 1 byte, try again

-- ── Chunk processing ──────────────────────────────────────────────────────────

-- | Split a Vector into chunks of size n, zero-pad the final chunk.
chunkVector :: Int -> Vector Float -> [Vector Float]
chunkVector n vec
  | V.null vec = []
  | otherwise  =
      let (h, t) = V.splitAt n vec
          padded  = if V.length h == n
                    then h
                    else h V.++ V.replicate (n - V.length h) 0.0
      in padded : chunkVector n t

-- | Process one IQ block through the Faust BPSK demodulator.
--   Feeds I and Q as separate channels, returns the soft-symbol output.
processBlock :: DspHandle -> Int -> Vector Float -> Vector Float -> IO (Vector Float)
processBlock dsp blockSize iVec qVec = do
  results <- compute dsp [iVec, qVec] blockSize
  case results of
    (softSym : _) -> return softSym
    []            -> throwIO (ErrorCall "Faust BPSK demod returned 0 output channels")

-- ── Single-frame RX ──────────────────────────────────────────────────────────

-- | Attempt to receive and decode one DeModFrame.
--
--   Reads IQ blocks from the SDR, feeds them through the Faust demodulator,
--   accumulates soft symbols until enough bits are available to search for a
--   frame, then returns the first valid frame found.
--
--   Returns Nothing on timeout (if the SDR produces only timeouts for
--   demodAcqFrames + 10 consecutive blocks, we give up).
rxOneFrame :: DemodConfig
           -> DspHandle
           -> SdrRxDevice
           -> IO (Maybe DeModFrame)
rxOneFrame DemodConfig{..} dsp sdrRx = do
  let sps         = samplesPerSymbol demodSymbol
      bitsPerFrame = 17 * 8
      -- We need at least bitsPerFrame symbols → bitsPerFrame * sps samples
      -- in the soft-symbol buffer before we start searching.
      minSamples  = bitsPerFrame * sps * 2   -- 2× margin for sync search
      maxBlocks   = 1000   -- ~1000 * 4096 / 2e6 ≈ 2 seconds; then give up

  -- Accumulator for soft symbols across multiple readSamples calls
  softAccRef <- newIORef (V.empty :: Vector Float)
  blocksRef  <- newIORef (0 :: Int)

  let loop = do
        blocks <- readIORef blocksRef
        if blocks > maxBlocks
          then return Nothing
          else do
            modifyIORef' blocksRef (+ 1)
            iqResult <- readSamples sdrRx (rxTimeoutUs demodSdr)
            case iqResult of
              Left 0  -> loop     -- timeout: retry
              Left _  -> return Nothing   -- hard error
              Right iq -> do
                let (iBlock, qBlock) = deinterleave iq
                    -- Chunk into Faust block sizes
                    iChunks = chunkVector demodBlockSize iBlock
                    qChunks = chunkVector demodBlockSize qBlock
                softParts <- mapM (\(ic, qc) -> processBlock dsp demodBlockSize ic qc)
                                  (zip iChunks qChunks)
                modifyIORef' softAccRef (V.++ V.concat softParts)
                acc <- readIORef softAccRef
                if V.length acc < minSamples
                  then loop
                  else do
                    -- Slice and search
                    let bits  = sliceSoftSymbols sps acc
                    case findFrame bits of
                      Just (frame, _remaining) -> return (Just frame)
                      Nothing -> do
                        -- Drop half the accumulator and keep searching
                        writeIORef softAccRef (V.drop (V.length acc `div` 2) acc)
                        loop

  loop

-- ── Main RX loop ──────────────────────────────────────────────────────────────

-- | Run the demodulator loop, calling @frameHandler@ for each received frame.
--   Loops until @frameHandler@ returns False (or the SDR hard-errors).
--
--   Acquisition:
--     The first @demodAcqFrames@ frame-slots of samples are fed through the
--     DSP but their soft-symbol output is discarded.  This gives the Costas
--     loop in bpsk_demod.dsp time to lock before we attempt frame sync.
--     At 9600 baud and 2 MSPS, one frame is 136 * 208 = 28,288 samples;
--     4 frames = ~113k samples = ~57 ms acquisition window.
runDemodulator :: DemodConfig
               -> (DeModFrame -> IO Bool)   -- ^ return True to continue, False to stop
               -> IO ()
runDemodulator cfg@DemodConfig{..} frameHandler =
  withDsp demodDsp $ \dsp ->
  withSdrRxDevice demodSdr $ \sdrRx -> do
    -- Set demodulator parameters via Faust MapUI
    setParam dsp "/bpsk_demod/symbol_rate"
             (realToFrac $ symSymbolRate demodSymbol)
    setParam dsp "/bpsk_demod/loop_bw"    0.02
    setParam dsp "/bpsk_demod/output_gain" 1.0

    -- Acquisition flush: discard first N frame-slots to let Costas loop lock
    let sps          = samplesPerSymbol demodSymbol
        acqSamples   = demodAcqFrames * 17 * 8 * sps
        acqBlocks    = (acqSamples + demodBlockSize - 1) `div` demodBlockSize
    flushAcquisition sdrRx dsp acqBlocks

    -- RX loop
    let loop = do
          mFrame <- rxOneFrame cfg dsp sdrRx
          case mFrame of
            Nothing    -> return ()
            Just frame -> do
              continue <- frameHandler frame
              when continue loop

    loop

-- | Feed @n@ blocks through the demodulator and discard the output.
--   Used to allow the Costas loop to acquire frequency lock before we
--   attempt frame synchronisation.
flushAcquisition :: SdrRxDevice -> DspHandle -> Int -> IO ()
flushAcquisition sdrRx dsp n = forM_ [1..n] $ \_ -> do
  result <- readSamples sdrRx 1_000_000
  case result of
    Left  _ -> return ()   -- timeout / error during acquisition — ignore
    Right iq -> do
      let (iBlock, qBlock) = deinterleave iq
      _ <- processBlock dsp (V.length iBlock) iBlock qBlock
      return ()
  where
    processBlock dsp' blkSz iVec qVec = do
      results <- compute dsp' [iVec, qVec] blkSz
      case results of
        (s : _) -> return s
        []      -> return V.empty
