-- DCF.Modulator — top-level TX pipeline
-- DeMoD LLC
--
-- Wires together:
--   1. Frame construction (DeModFrame)
--   2. BPSK symbol encoding (DCF.Transport.Symbol)
--   3. Faust DSP compute — pulse shaping + NCO + PM (DCF.Faust.DSP)
--   4. SoapySDR IQ stream write (DCF.SDR.Device)
--
-- Data flow:
--
--   DeModFrame
--       │ encodeFrame (17 bytes)
--       ▼
--   ByteString
--       │ encodeBpsk → Vector Float [symbol stream, upsampled]
--       ▼
--   Faust inputs[0]  ──► compute(blockSize) ──► outputs[0]=I, outputs[1]=Q
--                                                      │
--                                               interleave I/Q
--                                                      │
--                                               SoapySDR writeCF32

{-# LANGUAGE RecordWildCards #-}

module DCF.Modulator
  ( ModulatorConfig (..)
  , defaultModulatorConfig
  , runModulator
  , txFrame
  ) where

import Control.Exception        (throwIO, ErrorCall (..))
import Control.Monad            (when, forM_, unless)
import Data.IORef               (newIORef, readIORef, modifyIORef')
import Data.Time.Clock.POSIX    (getPOSIXTime)
import Data.Vector.Storable     (Vector)
import qualified Data.Vector.Storable as V
import Data.Word                (Word16, Word32)

import DCF.Faust.DSP            (DspConfig (..), DspHandle, withDsp, compute, setParam)
import DCF.SDR.Device           (SdrConfig (..), SdrDevice, withSdrDevice, writeSamples)
import DCF.Transport.Frame
import DCF.Transport.Symbol

-- ── Config ─────────────────────────────────────────────────────────────────────

data ModulatorConfig = ModulatorConfig
  { modDsp       :: DspConfig
  , modSdr       :: SdrConfig
  , modSymbol    :: SymbolConfig
  , modSrcId     :: Word16
  , modDstId     :: Word16
  , modVersion   :: Word8     -- ^ frame version nibble (0–15)
  , modBlockSize :: Int       -- ^ Faust compute block size (samples)
  } deriving (Show)

defaultModulatorConfig :: ModulatorConfig
defaultModulatorConfig = ModulatorConfig
  { modDsp    = DspConfig
      { dspSampleRate    = 2_000_000
      , dspExpectInputs  = 1
      , dspExpectOutputs = 2
      }
  , modSdr    = SdrConfig
      { sdrDriver       = "rtlsdr"
      , sdrCenterFreqHz = 433.92e6
      , sdrSampleRateHz = 2e6
      , sdrGainDb       = 20.0
      , sdrTimeoutUs    = 1_000_000
      }
  , modSymbol = defaultSymbolConfig
  , modSrcId     = 0x0001
  , modDstId     = 0xFFFF    -- broadcast
  , modVersion   = 1
  , modBlockSize = 1024
  }

-- ── Timestamp helper ──────────────────────────────────────────────────────────

-- | Get current time as a 24-bit microsecond offset (wraps ~16.7s).
timestampUs24 :: IO Word32
timestampUs24 = do
  t <- getPOSIXTime
  let us = round (t * 1_000_000) :: Integer
  return $ fromIntegral (us `mod` 0x1000000)

-- ── Single-frame TX ───────────────────────────────────────────────────────────

-- | Encode one DeModFrame and write the full IQ burst to the SDR.
--   Returns number of IQ samples written.
txFrame :: ModulatorConfig
        -> DspHandle
        -> SdrDevice
        -> DeModFrame
        -> IO Int
txFrame ModulatorConfig{..} dsp sdr frame = do
  -- 1. Encode frame → symbol stream (Vector Float)
  let symVec = frameToSymbols modSymbol frame
      symLen = V.length symVec

  -- 2. Feed symbols through Faust in modBlockSize chunks
  --    Faust outputs I and Q channel vectors.
  let chunks  = chunkVector modBlockSize symVec
  iqParts <- mapM (processChunk dsp) chunks

  -- 3. Interleave I/Q → CF32 and write to SDR
  let iqInterleaved = V.concat (map interleaveIQ iqParts)
  writeSamples sdr iqInterleaved

-- ── Chunk processing ──────────────────────────────────────────────────────────

-- | Run one block through Faust. Returns (iVec, qVec).
processChunk :: DspHandle -> Vector Float -> IO (Vector Float, Vector Float)
processChunk dsp symChunk = do
  outChans <- compute dsp [symChunk] (V.length symChunk)
  case outChans of
    (iVec : qVec : _) -> return (iVec, qVec)
    _ -> throwIO (ErrorCall "Faust DSP returned fewer than 2 output channels")

-- | Interleave I and Q vectors → [I0, Q0, I1, Q1, ...]
interleaveIQ :: (Vector Float, Vector Float) -> Vector Float
interleaveIQ (iVec, qVec) =
  V.generate (2 * V.length iVec) $ \n ->
    if even n
      then iVec V.! (n `div` 2)
      else qVec V.! (n `div` 2)

-- | Split a vector into chunks of size n (last chunk may be smaller).
chunkVector :: Int -> Vector Float -> [Vector Float]
chunkVector n vec
  | V.null vec = []
  | otherwise  =
      let (h, t) = V.splitAt n vec
      in h : chunkVector n t

-- ── Main TX loop ──────────────────────────────────────────────────────────────

-- | Run the modulator loop, calling frameSource for each new frame.
--   Loops until frameSource returns Nothing.
runModulator :: ModulatorConfig
             -> IO (Maybe (DeModFrame, [Byte]))  -- ^ frame source; Nothing = stop
             -> IO ()
runModulator cfg frameSource =
  withDsp (modDsp cfg) $ \dsp ->
    withSdrDevice (modSdr cfg) $ \sdr -> do
      -- Set Faust parameters from config
      setParam dsp "/modulator/carrier_freq"
               (realToFrac $ sdrCenterFreqHz (modSdr cfg) / (sdrSampleRateHz (modSdr cfg) / 2))
      setParam dsp "/modulator/symbol_rate"
               (realToFrac $ symSymbolRate (modSymbol cfg))

      seqRef <- newIORef (0 :: Word16)

      let loop = do
            mFrame <- frameSource
            case mFrame of
              Nothing -> return ()
              Just (baseFrame, _extra) -> do
                seq <- readIORef seqRef
                modifyIORef' seqRef (+1)
                ts <- timestampUs24

                let frame = baseFrame
                      { frameSeq         = seq
                      , frameTimestampUs = ts
                      , frameSrcId       = modSrcId cfg
                      , frameDstId       = modDstId cfg
                      , frameVersion     = modVersion cfg
                      }
                _written <- txFrame cfg dsp sdr frame
                loop

      loop

-- Type alias to avoid importing Data.Word twice
type Byte = ()
