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
--
-- BUG FIXES vs previous version:
--   1. `type Byte = ()` was a phantom placeholder leaking into the public
--      API of runModulator.  The frame source now yields `Maybe DeModFrame`
--      directly — no phantom extra field.
--   2. chunkVector could produce a short final chunk (length < modBlockSize).
--      Faust's vectorised compute (-vec -vs 256) processes fixed-size blocks;
--      feeding a short buffer writes past the allocated output buffers.
--      Fixed: padChunk zero-pads the last chunk to modBlockSize before calling
--      compute, then truncates the output to the original chunk length.
--   3. writeSamples was called with `sdrTimeoutUs defaultSdrConfig` instead
--      of the actual timeout from the user-supplied ModulatorConfig.
--      Fixed: pass modSdr cfg's sdrTimeoutUs explicitly.

{-# LANGUAGE RecordWildCards #-}

module DCF.Modulator
  ( ModulatorConfig (..)
  , defaultModulatorConfig
  , runModulator
  , txFrame
  ) where

import Control.Exception          (throwIO, ErrorCall (..))
import Control.Monad              (when)
import Data.IORef                 (newIORef, readIORef, modifyIORef')
import Data.Time.Clock.POSIX      (getPOSIXTime)
import Data.Vector.Storable       (Vector)
import qualified Data.Vector.Storable as V
import Data.Word                  (Word16, Word32, Word8)
import Foreign.C.Types            (CLong (..))

import DCF.Faust.DSP   (DspConfig (..), DspHandle, withDsp, compute, setParam)
import DCF.SDR.Device  (SdrConfig (..), SdrDevice, withSdrDevice, writeSamples)
import DCF.Transport.Frame
import DCF.Transport.Symbol

-- ── Config ────────────────────────────────────────────────────────────────────

data ModulatorConfig = ModulatorConfig
  { modDsp       :: DspConfig
  , modSdr       :: SdrConfig
  , modSymbol    :: SymbolConfig
  , modSrcId     :: Word16
  , modDstId     :: Word16
  , modVersion   :: Word8     -- ^ frame version nibble (0–15)
  , modBlockSize :: Int       -- ^ Faust compute block size (samples)
                              --   Must be a multiple of the Faust vector size
                              --   (256 for -vs 256).  Default: 1024.
  } deriving (Show)

defaultModulatorConfig :: ModulatorConfig
defaultModulatorConfig = ModulatorConfig
  { modDsp = DspConfig
      { dspSampleRate    = 2_000_000
      , dspExpectInputs  = 1
      , dspExpectOutputs = 2
      }
  , modSdr = SdrConfig
      { sdrDriver       = "rtlsdr"
      , sdrCenterFreqHz = 433.92e6
      , sdrSampleRateHz = 2e6
      , sdrGainDb       = 20.0
      , sdrTimeoutUs    = 1_000_000
      }
  , modSymbol    = defaultSymbolConfig
  , modSrcId     = 0x0001
  , modDstId     = 0xFFFF    -- broadcast
  , modVersion   = 1
  , modBlockSize = 1024      -- 4 × Faust vector size (256)
  }

-- ── Timestamp helper ──────────────────────────────────────────────────────────

-- | Get current time as a 24-bit microsecond offset (wraps ~16.7 s).
timestampUs24 :: IO Word32
timestampUs24 = do
  t <- getPOSIXTime
  let us = round (t * 1_000_000) :: Integer
  return $ fromIntegral (us `mod` 0x1000000)

-- ── Chunk helpers ─────────────────────────────────────────────────────────────

-- | Split a vector into chunks of size n.  Last chunk may be shorter.
chunkVector :: Int -> Vector Float -> [Vector Float]
chunkVector n vec
  | V.null vec = []
  | otherwise  = let (h, t) = V.splitAt n vec
                 in h : chunkVector n t

-- | Zero-pad a chunk to exactly `blockSize` samples so that Faust's
--   vectorised compute always receives a full block.
--   Returns (padded_input, valid_output_length).
padChunk :: Int -> Vector Float -> (Vector Float, Int)
padChunk blockSize chunk =
  let len = V.length chunk
  in if len == blockSize
     then (chunk, blockSize)
     else (chunk V.++ V.replicate (blockSize - len) 0.0, len)

-- ── Single-frame TX ───────────────────────────────────────────────────────────

-- | Encode one DeModFrame and write the full IQ burst to the SDR.
--   Returns number of IQ sample pairs written.
txFrame :: ModulatorConfig
        -> DspHandle
        -> SdrDevice
        -> DeModFrame
        -> IO Int
txFrame ModulatorConfig{..} dsp sdr frame = do
  let symVec  = frameToSymbols modSymbol frame
      chunks  = chunkVector modBlockSize symVec

  -- Process each chunk; zero-pad final short chunk so Faust always sees
  -- a full modBlockSize block.  Truncate output to original valid length.
  iqParts <- mapM (processChunk dsp modBlockSize) chunks

  let iqInterleaved = V.concat (map interleaveIQ iqParts)
  writeSamples sdr iqInterleaved (sdrTimeoutUs modSdr)

-- ── Chunk processing ──────────────────────────────────────────────────────────

-- | Run one block through Faust with zero-padding safety.
processChunk :: DspHandle -> Int -> Vector Float -> IO (Vector Float, Vector Float)
processChunk dsp blockSize chunk = do
  let (paddedInput, validLen) = padChunk blockSize chunk
  outChans <- compute dsp [paddedInput] blockSize
  case outChans of
    (iVec : qVec : _) ->
      -- Truncate output to validLen samples to match the real symbol data
      return (V.take validLen iVec, V.take validLen qVec)
    _ -> throwIO (ErrorCall "Faust DSP returned fewer than 2 output channels")

-- | Interleave I and Q vectors → [I0, Q0, I1, Q1, ...]
interleaveIQ :: (Vector Float, Vector Float) -> Vector Float
interleaveIQ (iVec, qVec) =
  V.generate (2 * V.length iVec) $ \n ->
    if even n
      then iVec V.! (n `div` 2)
      else qVec V.! (n `div` 2)

-- ── Main TX loop ──────────────────────────────────────────────────────────────

-- | Run the modulator loop, calling frameSource for each new frame.
--   Loops until frameSource returns Nothing.
--
--   The frame source yields @Maybe DeModFrame@.  seq, timestamp, src_id,
--   dst_id, and version are overwritten by runModulator — the caller only
--   needs to set frameType and framePayload.
runModulator :: ModulatorConfig
             -> IO (Maybe DeModFrame)   -- ^ frame source; Nothing = stop
             -> IO ()
runModulator cfg frameSource =
  withDsp (modDsp cfg) $ \dsp ->
    withSdrDevice (modSdr cfg) $ \sdr -> do
      -- Normalise carrier freq: SDR hardware uses absolute Hz; Faust
      -- hslider uses normalised 0..0.5 (0 to SR/2).
      setParam dsp "/modulator/carrier_freq"
               (realToFrac $ sdrCenterFreqHz (modSdr cfg)
                           / (sdrSampleRateHz (modSdr cfg) / 2))
      setParam dsp "/modulator/symbol_rate"
               (realToFrac $ symSymbolRate (modSymbol cfg))

      seqRef <- newIORef (0 :: Word16)

      let loop = do
            mFrame <- frameSource
            case mFrame of
              Nothing    -> return ()
              Just baseFrame -> do
                seq' <- readIORef seqRef
                modifyIORef' seqRef (+ 1)
                ts <- timestampUs24

                let frame = baseFrame
                      { frameSeq         = seq'
                      , frameTimestampUs = ts
                      , frameSrcId       = modSrcId cfg
                      , frameDstId       = modDstId cfg
                      , frameVersion     = modVersion cfg
                      }
                _written <- txFrame cfg dsp sdr frame
                loop

      loop
