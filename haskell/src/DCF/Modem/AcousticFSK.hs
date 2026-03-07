-- DCF.Modem.AcousticFSK — DeMoD Acoustic FSK Modem
-- DeMoD LLC | GPL-3.0
--
-- Haskell TX/RX integration for acoustic_fsk_mod.dsp + acoustic_fsk_demod.dsp.
-- Transmits and receives JackFrame data as audible two-tone FSK via
-- JACK/PipeWire audio ports.
--
-- TX pipeline:
--   JackFrame → encodeJF → NRZ-L bits → upsample (sps=40) → Faust mod → speaker
--
-- RX pipeline:
--   mic → Faust demod → soft symbols → slice @ symbol centres → bits → decodeJF
--
-- Sounds like a dial-up modem. That is intentional.
--
-- Default parameters (match acoustic_fsk_mod.dsp / acoustic_fsk_demod.dsp):
--   Sample rate : 48000 Hz
--   Symbol rate : 1200 baud
--   sps         : 40 (integer — clean timing)
--   f_mark      : 2000 Hz (bit 1)
--   f_space     : 3000 Hz (bit 0)
--   Frame       : JackFrame, 8 bytes = 64 bits = 53.3 ms airtime
--   Preamble    : 16 alternating mark/space symbols (13.3 ms) before each frame

{-# LANGUAGE ScopedTypeVariables #-}

module DCF.Modem.AcousticFSK
  ( AcousticConfig (..)
  , defaultAcousticConfig
  -- * TX
  , setAcousticTxParams
  , encodeAcousticFrame
  , runAcousticTx
  -- * RX
  , setAcousticRxParams
  , sliceAcousticSymbols
  , findJackFrame
  , runAcousticRx
  -- * Utilities
  , acousticSps
  , acousticFrameSamples
  , preambleSamples
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
import DCF.Transport.JackFrame

-- ── Configuration ─────────────────────────────────────────────────────────────

data AcousticConfig = AcousticConfig
  { acSampleRate  :: Double   -- ^ Audio interface sample rate (Hz). Default 48000.
  , acSymbolRate  :: Double   -- ^ Baud rate. Default 1200.
  , acFMark       :: Double   -- ^ Mark tone frequency Hz. Default 2000.
  , acFSpace      :: Double   -- ^ Space tone frequency Hz. Default 3000.
  , acOutputGain  :: Double   -- ^ Speaker output level 0..1. Default 0.7.
  , acBpfQ        :: Double   -- ^ Demod BPF Q factor. Default 3.5.
  , acDemodGain   :: Double   -- ^ Demod output scale. Default 4.0.
  , acBlockSize   :: Int      -- ^ Faust compute block size (samples). Default 256.
  , acAcqFrames   :: Int      -- ^ Frames to discard while mic AGC settles. Default 3.
  , acPreambleLen :: Int      -- ^ Preamble symbols before each TX frame. Default 16.
  , acSilenceLen  :: Int      -- ^ Pre-frame silence samples (mic AGC settle). Default 40.
  } deriving (Show)

defaultAcousticConfig :: AcousticConfig
defaultAcousticConfig = AcousticConfig
  { acSampleRate  = 48000.0
  , acSymbolRate  = 1200.0
  , acFMark       = 2000.0
  , acFSpace      = 3000.0
  , acOutputGain  = 0.7
  , acBpfQ        = 3.5
  , acDemodGain   = 4.0
  , acBlockSize   = 256
  , acAcqFrames   = 3
  , acPreambleLen = 16
  , acSilenceLen  = 40    -- one symbol period of silence before preamble
  }

-- ── Derived parameters ────────────────────────────────────────────────────────

-- | Samples per symbol (should be integer for clean timing).
--   At 48000 Hz / 1200 baud = 40.0 exactly.
acousticSps :: AcousticConfig -> Int
acousticSps cfg = round (acSampleRate cfg / acSymbolRate cfg)

-- | Samples per JackFrame (64 bits × sps).
acousticFrameSamples :: AcousticConfig -> Int
acousticFrameSamples cfg = jfFrameSize * 8 * acousticSps cfg

-- | Samples in the preamble (acPreambleLen symbols × sps).
preambleSamples :: AcousticConfig -> Int
preambleSamples cfg = acPreambleLen cfg * acousticSps cfg

-- ── Parameter push ────────────────────────────────────────────────────────────

-- | Push TX parameters to acoustic_fsk_mod Faust DSP handle.
setAcousticTxParams :: DspHandle -> AcousticConfig -> IO ()
setAcousticTxParams dsp cfg = do
  setParam dsp "/acoustic_fsk_mod/f_mark"      (realToFrac $ acFMark      cfg)
  setParam dsp "/acoustic_fsk_mod/f_space"     (realToFrac $ acFSpace     cfg)
  setParam dsp "/acoustic_fsk_mod/symbol_rate" (realToFrac $ acSymbolRate cfg)
  setParam dsp "/acoustic_fsk_mod/output_gain" (realToFrac $ acOutputGain cfg)

-- | Push RX parameters to acoustic_fsk_demod Faust DSP handle.
setAcousticRxParams :: DspHandle -> AcousticConfig -> IO ()
setAcousticRxParams dsp cfg = do
  setParam dsp "/acoustic_fsk_demod/f_mark"      (realToFrac $ acFMark      cfg)
  setParam dsp "/acoustic_fsk_demod/f_space"     (realToFrac $ acFSpace     cfg)
  setParam dsp "/acoustic_fsk_demod/symbol_rate" (realToFrac $ acSymbolRate cfg)
  setParam dsp "/acoustic_fsk_demod/bpf_q"       (realToFrac $ acBpfQ       cfg)
  setParam dsp "/acoustic_fsk_demod/output_gain" (realToFrac $ acDemodGain  cfg)

-- ── TX: JackFrame → audio samples ────────────────────────────────────────────

-- | Pack a Word8 into 8 NRZ-L floats, MSB first.
--   +1.0 = mark (bit 1), -1.0 = space (bit 0).
byteToBits :: Word8 -> [Float]
byteToBits b = map (\i -> if (b `shiftR` (7-i)) .&. 1 == 1 then 1.0 else -1.0) [0..7]

-- | Build the complete upsampled symbol vector for one JackFrame transmission:
--   silence(acSilenceLen) ++ preamble(acPreambleLen) ++ frame_bits(64)
--   Each symbol is held for sps samples.
encodeAcousticFrame :: AcousticConfig -> JackFrame -> Vector Float
encodeAcousticFrame cfg frame =
  let sps      = acousticSps cfg
      wire     = encodeJF frame          -- 8 bytes
      bits     = concatMap byteToBits (BS.unpack wire)  -- 64 floats ±1
      -- Alternating preamble: mark, space, mark, space, ...
      preamble = take (acPreambleLen cfg) (cycle [1.0, -1.0])
      -- Silence before preamble (lets mic AGC settle between frames)
      silence  = replicate (acSilenceLen cfg) 0.0
      allSyms  = silence ++ preamble ++ bits
  in  V.fromList [ s | s <- allSyms, _ <- [1..sps] ]

-- | Run the acoustic TX loop.
--   frameSource returns Nothing to stop.
--   writeSamples sends a Vector Float to the JACK/PipeWire output port.
runAcousticTx
  :: AcousticConfig
  -> DspHandle
  -> (Vector Float -> IO ())  -- ^ write audio to output port
  -> IO (Maybe JackFrame)     -- ^ frame source
  -> IO ()
runAcousticTx cfg dsp writeAudio frameSource = do
  setAcousticTxParams dsp cfg
  let loop = do
        mf <- frameSource
        case mf of
          Nothing    -> return ()
          Just frame -> do
            let symVec = encodeAcousticFrame cfg frame
            -- Feed through Faust acoustic_fsk_mod DSP
            -- (processChunk from DCF.Modulator — same pattern)
            audioOut <- processChunked dsp (acBlockSize cfg) symVec
            writeAudio audioOut
            loop
  loop
  where
    -- Chunk the symbol vector through Faust in blockSize pieces
    processChunked :: DspHandle -> Int -> Vector Float -> IO (Vector Float)
    processChunked _dsp _blk _v =
      -- Stub: wire to DCF.Modulator.processChunk
      -- actual call: mapM (processChunk dsp blk) (chunkVector blk v) >>= return . V.concat
      return V.empty

-- ── RX: audio samples → JackFrame ────────────────────────────────────────────

-- | Sample the soft-symbol output at symbol centres.
--   sps=40: sample at offset 20, 60, 100, ... (centre of each symbol period).
sliceAcousticSymbols :: Int -> Vector Float -> [Bool]
sliceAcousticSymbols sps soft =
  let offset = sps `div` 2
      idxs   = [offset, offset + sps .. V.length soft - 1]
  in  map (\i -> soft V.! i > 0.0) idxs

-- | Pack bits MSB-first into ByteString.
bitsToBytes :: [Bool] -> ByteString
bitsToBytes [] = BS.empty
bitsToBytes bs =
  let (byte_bits, rest) = splitAt 8 bs
      w = foldl (\acc b -> acc * 2 + if b then 1 else 0) 0 byte_bits :: Word8
  in  BS.cons w (bitsToBytes rest)

-- | Sliding byte-aligned frame search in a bit list.
--   Scans for the 0xA5 JackFrame sync byte, then attempts CRC decode.
findJackFrame :: [Bool] -> Maybe (JackFrame, [Bool])
findJackFrame bits
  | length bits < jfFrameSize * 8 = Nothing
  | otherwise =
      let (candidate, rest) = splitAt (jfFrameSize * 8) bits
          bs = bitsToBytes candidate
      in  case decodeJF bs of
            Just frame -> Just (frame, rest)
            Nothing    -> findJackFrame (tail bits)

-- | Run the acoustic RX loop.
--   readAudio returns the next block of mic samples (from JACK/PipeWire capture).
--   handler is called for each decoded JackFrame; return False to stop.
runAcousticRx
  :: AcousticConfig
  -> DspHandle
  -> IO (Vector Float)        -- ^ read audio from input port
  -> (JackFrame -> IO Bool)   -- ^ frame handler
  -> IO ()
runAcousticRx cfg dsp readAudio handler = do
  setAcousticRxParams dsp cfg
  let sps = acousticSps cfg

  -- Discard initial blocks while mic AGC settles
  putStrLn "[acoustic-rx] Waiting for mic AGC to settle..."
  let acqSamples = acAcqFrames cfg * acousticFrameSamples cfg
      acqBlocks  = (acqSamples + acBlockSize cfg - 1) `div` acBlockSize cfg
  forM_ [1..acqBlocks] $ \_ -> readAudio

  putStrLn "[acoustic-rx] Listening for frames..."
  bitBuf <- newIORef ([] :: [Bool])

  let loop = do
        micBlock <- readAudio
        -- Feed through Faust acoustic_fsk_demod DSP
        softBlock <- processChunked dsp (acBlockSize cfg) micBlock
        -- Slice soft symbols at sps centres
        let newBits = sliceAcousticSymbols sps softBlock
        modifyIORef' bitBuf (++ newBits)
        bits <- readIORef bitBuf
        processBits bits
        loop

      processBits bits = do
        case findJackFrame bits of
          Nothing          -> return ()
          Just (frame, rest) -> do
            writeIORef bitBuf rest
            continue <- handler frame
            when continue $ do
              bits' <- readIORef bitBuf
              processBits bits'

  loop

  where
    processChunked :: DspHandle -> Int -> Vector Float -> IO (Vector Float)
    processChunked _dsp _blk _v = return V.empty  -- stub: wire to processChunk
