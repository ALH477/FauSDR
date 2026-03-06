-- DCF.Modem.QPSK — Gray-coded QPSK symbol mapper / de-mapper
-- DeMoD LLC
--
-- Converts between bit streams and QPSK (I, Q) symbol pairs.
--
-- Constellation (Gray-coded, per ITU-T V.34 and DVB-S standard):
--
--   Bits  Phase   I     Q
--   00    45°    +1    +1
--   01    135°   -1    +1
--   11    225°   -1    -1
--   10    315°   +1    -1
--
-- Gray coding: adjacent symbols differ by exactly 1 bit.
-- This minimises bit errors when the slicer makes a 1-symbol error in noise.
--
-- Mapping:
--   MSB → I channel    (+1 if MSB=0, -1 if MSB=1)
--   LSB → Q channel    (+1 if LSB=0, -1 if LSB=1)
--
-- The encoded symbol pairs are fed as two parallel ±1.0 float streams
-- to the Faust qpsk_mod.dsp:
--   Faust input[0] = I stream
--   Faust input[1] = Q stream
--
-- After Faust RRC shaping and carrier modulation, the modulated IQ signal
-- is written to the SDR TX stream.
--
-- De-mapping (RX side):
--   After the Faust qpsk_demod.dsp Costas loop and RRC matched filter,
--   Faust outputs two soft-symbol channels:
--     output[0] = I soft symbols
--     output[1] = Q soft symbols
--   We slice independently (threshold 0), reconstruct pairs, then
--   Gray-decode to recover the original bit stream.

module DCF.Modem.QPSK
  ( -- * TX: bits → symbol pairs
    qpskEncode
  , qpskEncodeByteString
    -- * RX: soft symbol pairs → bits
  , qpskDecode
  , qpskDecodeToByteString
    -- * Config
  , QpskConfig (..)
  , defaultQpskConfig
  ) where

import Data.Bits              (testBit, shiftL, (.|.))
import Data.ByteString        (ByteString)
import qualified Data.ByteString as BS
import Data.Vector.Storable   (Vector)
import qualified Data.Vector.Storable as V
import Data.Word              (Word8)

-- ── Config ────────────────────────────────────────────────────────────────────

data QpskConfig = QpskConfig
  { qpskSampleRate  :: Double
  , qpskSymbolRate  :: Double   -- ^ QPSK: 2 bits/symbol → bit_rate = 2 * symbolRate
  } deriving (Show, Eq)

defaultQpskConfig :: QpskConfig
defaultQpskConfig = QpskConfig
  { qpskSampleRate = 2_000_000
  , qpskSymbolRate = 28_800    -- 57.6 kbps at 28.8 kbaud
  }

-- ── TX: Gray-coded QPSK symbol mapper ────────────────────────────────────────

-- | Map two bits to a Gray-coded QPSK symbol pair (I, Q) ∈ {±1}².
--
--   bit_msb → I:  0 → +1.0,  1 → -1.0
--   bit_lsb → Q:  0 → +1.0,  1 → -1.0
mapSymbol :: Bool -> Bool -> (Float, Float)
mapSymbol msb lsb =
  let i = if msb then -1.0 else 1.0
      q = if lsb then -1.0 else 1.0
  in (i, q)

-- | Encode a list of bits into parallel I and Q symbol vectors.
--   Input bits are consumed in pairs (MSB first within each pair).
--   If the input has an odd number of bits, the last bit is zero-padded.
--
--   Both output vectors have length @ceil(length bits / 2)@.
--   Feed them to DCF.Faust.DSP compute as [iVec, qVec] for qpsk_mod.dsp.
qpskEncode :: [Bool] -> (Vector Float, Vector Float)
qpskEncode bits =
  let padded = bits ++ [False]          -- ensure even length
      pairs  = takePairs padded
      syms   = map (uncurry mapSymbol) pairs
      iSyms  = map fst syms
      qSyms  = map snd syms
  in (V.fromList iSyms, V.fromList qSyms)
  where
    takePairs [] = []
    takePairs [b] = [(b, False)]
    takePairs (b0:b1:rest) = (b0, b1) : takePairs rest

-- | Encode a ByteString as a QPSK symbol stream, upsampled by samplesPerSymbol.
--
--   Each bit is held for @sps@ samples (rectangular pulse); Faust's RRC filter
--   in qpsk_mod.dsp convolves with the RRC kernel.
--
--   Returns (iVec, qVec) both of length 4 * BS.length bs * sps
--   (4 = 8 bits / 2 bits_per_symbol, but we still produce one sample per
--   upsampled symbol, so total = nSymbols * sps).
qpskEncodeByteString :: QpskConfig -> ByteString -> (Vector Float, Vector Float)
qpskEncodeByteString cfg bs =
  let sps    = samplesPerSymbol cfg
      bits   = bytesToBits bs
      (iRaw, qRaw) = qpskEncode bits
      nSym   = V.length iRaw
      upI    = V.generate (nSym * sps) (\n -> iRaw V.! (n `div` sps))
      upQ    = V.generate (nSym * sps) (\n -> qRaw V.! (n `div` sps))
  in (upI, upQ)
  where
    samplesPerSymbol c = floor (qpskSampleRate c / qpskSymbolRate c) :: Int

-- | Extract 8 bits from a ByteString, MSB first.
bytesToBits :: ByteString -> [Bool]
bytesToBits = concatMap byteToBits . BS.unpack
  where
    byteToBits b = [ testBit b (7 - i) | i <- [0..7] ]

-- ── RX: Gray-coded QPSK de-mapper ────────────────────────────────────────────

-- | Hard-slice a soft symbol: True = +1 (bit 0), False = -1 (bit 1).
sliceBit :: Float -> Bool
sliceBit x = x > 0.0

-- | De-map a pair of sliced I/Q soft symbols back to two bits.
--   Inverse of mapSymbol.
demapSymbol :: Float -> Float -> (Bool, Bool)
demapSymbol i q = (not (sliceBit i), not (sliceBit q))
  -- i > 0 → I=+1 → MSB=0 (not True = False) → bit 0
  -- i < 0 → I=-1 → MSB=1 → bit 1

-- | Decode parallel I and Q soft-symbol vectors into a bit list.
--   The two vectors must have the same length.
qpskDecode :: Vector Float   -- ^ I soft symbols (from Faust output[0])
           -> Vector Float   -- ^ Q soft symbols (from Faust output[1])
           -> [Bool]
qpskDecode iSoft qSoft =
  let n    = min (V.length iSoft) (V.length qSoft)
      syms = [ demapSymbol (iSoft V.! k) (qSoft V.! k) | k <- [0..n-1] ]
  in concatMap (\(msb, lsb) -> [msb, lsb]) syms

-- | Decode soft-symbol vectors into a ByteString.
--   Bits are packed MSB first within each byte.
--   Trailing bits that don't fill a complete byte are discarded.
qpskDecodeToByteString :: Vector Float -> Vector Float -> ByteString
qpskDecodeToByteString iSoft qSoft =
  BS.pack . bitsToBytes . qpskDecode iSoft $ qSoft
  where
    bitsToBytes [] = []
    bitsToBytes bs =
      let (byte_bits, rest) = splitAt 8 bs
      in if length byte_bits < 8
           then []
           else packByte byte_bits : bitsToBytes rest
    packByte :: [Bool] -> Word8
    packByte = foldl (\acc b -> acc `shiftL` 1 .|. if b then 1 else 0) 0
