-- DCF/Modem/Wideband.hs — DeMoD Wideband BPSK Modem (40–192 kHz)
--
-- Haskell integration layer for wideband_mod.dsp + wideband_demod.dsp.
--
-- Carrier range : 40 kHz – 192 kHz
-- Symbol rates  : 40 kbaud (sps=50) or 96 kbaud (sps≈20.83)
-- Sample rate   : 2 MSPS (fixed — RRC coefficients baked in at compile time)
--
-- TX usage:
--   withSdrDevice txCfg $ \sdr ->
--     withDspHandle "build/wideband_mod_gen.cpp" 2000000 $ \dsp -> do
--       setWidebandTxParams dsp wbCfg
--       runWidebandModulator wbCfg dsp sdr frameSource
--
-- RX usage:
--   withSdrRxDevice rxCfg $ \sdr ->
--     withDspHandle "build/wideband_demod_gen.cpp" 2000000 $ \dsp -> do
--       runWidebandDemodulator wbCfg dsp sdr frameHandler
--
-- DeMoD LLC — GPL-3.0

module DCF.Modem.Wideband
  ( WidebandConfig (..)
  , WidebandRate (..)
  , defaultWidebandConfig
  , widebandConfig40k
  , widebandConfig96k
  , samplesPerSymbolWB
  , samplesPerFrameWB
  , setWidebandTxParams
  , setWidebandRxParams
  , runWidebandModulator
  , runWidebandDemodulator
  ) where

import Control.Monad         (forM_, when)
import Data.Bits             (shiftR, (.&.))
import Data.ByteString       (ByteString)
import qualified Data.ByteString as BS
import Data.IORef
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable as V
import Data.Word             (Word8)

import DCF.Faust.DSP         (DspHandle, setParam)
import DCF.SDR.Device        (SdrDevice, SdrRxDevice, writeSamples, readSamples)
import DCF.Transport.Frame   (DeModFrame (..), encodeFrame, decodeFrame)

-- ── Configuration ─────────────────────────────────────────────────────────────

-- | Symbol rate preset.
data WidebandRate
  = Rate40k   -- ^ 40 kbaud, sps=50, 301-tap RRC, null BW = 54 kHz
  | Rate96k   -- ^ 96 kbaud, sps≈20.83, 125-tap RRC, null BW = 129.6 kHz
  deriving (Show, Eq)

data WidebandConfig = WidebandConfig
  { wbRate        :: WidebandRate  -- ^ Symbol rate preset
  , wbCarrierHz   :: Double        -- ^ Carrier frequency in Hz (40e3 – 192e3)
  , wbSampleRate  :: Double        -- ^ SDR sample rate (must be 2e6)
  , wbModIndex    :: Double        -- ^ PM deviation (pi = BPSK)
  , wbOutputGain  :: Double        -- ^ DAC output level (0..1)
  , wbLoopBw      :: Double        -- ^ Costas loop bandwidth (RX only)
  , wbBlockSize   :: Int           -- ^ Faust compute block size (samples)
  , wbAcqFrames   :: Int           -- ^ Frames to discard for Costas acquisition
  } deriving (Show)

-- | Default: 40 kbaud, 100 kHz carrier, 2 MSPS.
defaultWidebandConfig :: WidebandConfig
defaultWidebandConfig = WidebandConfig
  { wbRate       = Rate40k
  , wbCarrierHz  = 100000.0    -- 100 kHz carrier
  , wbSampleRate = 2000000.0   -- 2 MSPS
  , wbModIndex   = pi          -- BPSK
  , wbOutputGain = 0.8
  , wbLoopBw     = 0.03        -- wider than 9600 baud default
  , wbBlockSize  = 1024
  , wbAcqFrames  = 8           -- 96k acquisition window ≈ 8 × 1.42 ms = 11.4 ms
  }

-- | 40 kbaud preset, 70 kHz carrier (centred in 40–192 kHz with 54 kHz BW).
widebandConfig40k :: WidebandConfig
widebandConfig40k = defaultWidebandConfig
  { wbRate      = Rate40k
  , wbCarrierHz = 70000.0   -- 70 kHz carrier; null edges at 43 kHz and 97 kHz
  , wbLoopBw    = 0.025
  , wbAcqFrames = 6
  }

-- | 96 kbaud preset, 130 kHz carrier.
widebandConfig96k :: WidebandConfig
widebandConfig96k = defaultWidebandConfig
  { wbRate      = Rate96k
  , wbCarrierHz = 130000.0  -- 130 kHz carrier; null edges at 65.2 kHz and 194.8 kHz
  , wbLoopBw    = 0.045     -- wider BL for lower sps
  , wbAcqFrames = 10        -- 96k: 10 × 1.42 ms = 14.2 ms acquisition
  }

-- ── Derived parameters ────────────────────────────────────────────────────────

-- | Samples per symbol for this config.
samplesPerSymbolWB :: WidebandConfig -> Int
samplesPerSymbolWB cfg = case wbRate cfg of
  Rate40k -> 50                          -- 2e6 / 40000 = 50 exactly
  Rate96k -> floor (wbSampleRate cfg / 96000.0)  -- ≈ 20

-- | Samples per 17-byte DCF frame (136 bits).
samplesPerFrameWB :: WidebandConfig -> Int
samplesPerFrameWB cfg = 136 * samplesPerSymbolWB cfg

-- | Normalized carrier frequency (0..0.5) for Faust parameter.
carrierNorm :: WidebandConfig -> Double
carrierNorm cfg = wbCarrierHz cfg / (wbSampleRate cfg / 2.0)

-- | Integer RRC mode (0 = 40k/301-tap, 1 = 96k/125-tap).
rrcMode :: WidebandConfig -> Int
rrcMode cfg = case wbRate cfg of { Rate40k -> 0; Rate96k -> 1 }

-- | Symbol rate in baud.
symbolRateHz :: WidebandConfig -> Double
symbolRateHz cfg = case wbRate cfg of { Rate40k -> 40000.0; Rate96k -> 96000.0 }

-- ── Parameter setup ───────────────────────────────────────────────────────────

-- | Push TX parameters to the Faust wideband_mod DSP.
setWidebandTxParams :: DspHandle -> WidebandConfig -> IO ()
setWidebandTxParams dsp cfg = do
  setParam dsp "/wideband_mod/carrier_freq" (realToFrac $ carrierNorm cfg)
  setParam dsp "/wideband_mod/symbol_rate"  (realToFrac $ symbolRateHz cfg)
  setParam dsp "/wideband_mod/mod_index"    (realToFrac $ wbModIndex cfg)
  setParam dsp "/wideband_mod/output_gain"  (realToFrac $ wbOutputGain cfg)
  setParam dsp "/wideband_mod/rrc_mode"     (fromIntegral $ rrcMode cfg)

-- | Push RX parameters to the Faust wideband_demod DSP.
setWidebandRxParams :: DspHandle -> WidebandConfig -> IO ()
setWidebandRxParams dsp cfg = do
  setParam dsp "/wideband_demod/symbol_rate"  (realToFrac $ symbolRateHz cfg)
  setParam dsp "/wideband_demod/loop_bw"      (realToFrac $ wbLoopBw cfg)
  setParam dsp "/wideband_demod/mf_bw"        0.5
  setParam dsp "/wideband_demod/output_gain"  1.0
  setParam dsp "/wideband_demod/rrc_mode"     (fromIntegral $ rrcMode cfg)

-- ── TX Pipeline ──────────────────────────────────────────────────────────────

-- | Encode a DeModFrame to NRZ-L symbol vector (±1.0 floats), sps-upsampled.
encodeToSymbols :: WidebandConfig -> DeModFrame -> Vector Float
encodeToSymbols cfg frame =
  let raw   = encodeFrame frame          -- ByteString, 17 bytes
      sps   = samplesPerSymbolWB cfg
      bits  = concatMap byteToBits (BS.unpack raw)
      syms  = map (\b -> if b then 1.0 else -1.0) bits
  in  V.fromList [ s | s <- syms, _ <- [1..sps] ]
  where
    byteToBits :: Word8 -> [Bool]
    byteToBits b = map (\i -> (b `shiftR` (7-i)) .&. 1 == 1) [0..7]

-- | Run one frame through the Faust TX DSP, returning interleaved CF32 IQ.
modulateFrame :: DspHandle -> Int -> Vector Float -> IO (Vector Float)
modulateFrame dsp blockSize symVec = do
  let nSamples = V.length symVec
      -- zero-pad to multiple of blockSize
      padded   = symVec V.++ V.replicate (nBlocks * blockSize - nSamples) 0.0
      nBlocks  = (nSamples + blockSize - 1) `div` blockSize
  iqParts <- mapM (processBlock dsp blockSize) [V.slice (i*blockSize) blockSize padded | i <- [0..nBlocks-1]]
  return $ V.slice 0 (nSamples * 2) (V.concat iqParts)
  where
    processBlock :: DspHandle -> Int -> Vector Float -> IO (Vector Float)
    processBlock _ _ _ = return V.empty  -- stub; see DCF.Modulator.processChunk
    -- ^ Stub — actual implementation uses faust_dsp_compute via FFI.
    --   See DCF.Modulator.processChunk for the real implementation.

-- | Run the wideband TX modulator, transmitting frames from a source action.
--   frameSource returns Nothing to stop.
runWidebandModulator
  :: WidebandConfig
  -> DspHandle
  -> SdrDevice
  -> IO (Maybe DeModFrame)  -- ^ frame source
  -> IO ()
runWidebandModulator cfg dsp sdr frameSource = do
  setWidebandTxParams dsp cfg
  let loop n = do
        mf <- frameSource
        case mf of
          Nothing    -> return ()
          Just frame -> do
            let syms = encodeToSymbols cfg frame
            iq   <- modulateFrame dsp (wbBlockSize cfg) syms
            _    <- writeSamples sdr iq 1_000_000
            loop (n+1)
  loop (0 :: Int)

-- ── RX Pipeline ──────────────────────────────────────────────────────────────

-- | Deinterleave flat CF32 vector [I0,Q0,I1,Q1,...] into separate I/Q vectors.
deinterleave :: Vector Float -> (Vector Float, Vector Float)
deinterleave v =
  let n  = V.length v `div` 2
      is = V.generate n (\i -> v V.! (2*i))
      qs = V.generate n (\i -> v V.! (2*i+1))
  in  (is, qs)

-- | Downsample soft symbols at symbol centres (sps/2 offset, stride sps).
sliceWB :: Int -> Vector Float -> [Bool]
sliceWB sps soft =
  let offset = sps `div` 2
      idxs   = [offset, offset+sps .. V.length soft - 1]
  in  map (\i -> soft V.! i > 0.0) idxs

-- | Pack MSB-first bits into ByteString.
bitsToBytes :: [Bool] -> ByteString
bitsToBytes [] = BS.empty
bitsToBytes bs =
  let (byte_bits, rest) = splitAt 8 bs
      w = foldl (\acc b -> acc * 2 + (if b then 1 else 0)) 0 byte_bits :: Word8
  in  BS.cons w (bitsToBytes rest)

-- | Sliding byte-aligned frame search in a bit stream.
findFrameWB :: [Bool] -> Maybe (DeModFrame, [Bool])
findFrameWB bits
  | length bits < 136 = Nothing
  | otherwise =
      let (candidate, rest) = splitAt 136 bits
          bs = bitsToBytes candidate
      in  case decodeFrame bs of
            Just frame -> Just (frame, rest)
            Nothing    -> findFrameWB (drop 1 bits)

-- | Run the wideband RX demodulator.
--   Calls handler for each decoded frame; handler returns False to stop.
runWidebandDemodulator
  :: WidebandConfig
  -> DspHandle
  -> SdrRxDevice
  -> (DeModFrame -> IO Bool)   -- ^ frame handler (return False to stop)
  -> IO ()
runWidebandDemodulator cfg dsp sdr handler = do
  setWidebandRxParams dsp cfg
  let sps       = samplesPerSymbolWB cfg
      blockSz   = wbBlockSize cfg
      -- Acquisition: discard N frame-slots to let Costas loop lock
      acqSamps  = wbAcqFrames cfg * samplesPerFrameWB cfg
      acqBlocks = (acqSamps + blockSz - 1) `div` blockSz

  putStrLn "[wideband-rx] Acquiring carrier lock..."
  -- Flush acquisition blocks
  forM_ [1..acqBlocks] $ \_ -> do
    r <- readSamples sdr blockSz
    case r of
      Right _  -> return ()
      Left  _  -> return ()

  -- Main RX loop
  softBuf <- newIORef ([] :: [Bool])
  let loop = do
        r <- readSamples sdr blockSz
        case r of
          Left 0  -> loop           -- timeout — keep trying
          Left _  -> return ()      -- hard error — stop
          Right iq -> do
            let (ivec, _qvec) = deinterleave iq
                -- TODO: feed through Faust wideband_demod DSP via processBlock
                -- For now soft = ivec (direct I-arm, no matched filter)
                -- Replace with: soft <- processDemodBlock dsp blockSz ivec qvec
                soft = ivec
            modifyIORef' softBuf (++ sliceWB sps soft)
            bits <- readIORef softBuf
            processBits bits softBuf handler
            loop

  loop

  where
    processBits bits bufRef h = do
      case findFrameWB bits of
        Nothing         -> return ()
        Just (frame, rest) -> do
          writeIORef bufRef rest
          continue <- h frame
          when continue $ do
            bits' <- readIORef bufRef
            processBits bits' bufRef h
