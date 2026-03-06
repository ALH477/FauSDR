-- DCF.Transport.Symbol — BPSK symbol encoder
-- DeMoD LLC
--
-- Converts a 17-byte DeModFrame into a float vector suitable for Faust's
-- input channel 0 (the symbol stream / modulating signal).
--
-- Encoding:
--   bit 1 → +1.0  (NRZ-L)
--   bit 0 → -1.0
--
-- Each bit is held for `samplesPerSymbol` samples (rectangular pulse),
-- which Faust's pulse shaping filter will then convolve with the RRC kernel.
--
-- The full frame (17 bytes = 136 bits) produces a vector of length:
--   136 * samplesPerSymbol
--
-- With sampleRate=2e6 and symbolRate=9600:
--   samplesPerSymbol = 2_000_000 / 9_600 ≈ 208

module DCF.Transport.Symbol
  ( SymbolConfig (..)
  , defaultSymbolConfig
  , encodeBpsk
  , frameToSymbols
  , samplesPerFrame
  ) where

import Data.Bits             (testBit)
import Data.ByteString       (ByteString)
import qualified Data.ByteString as BS
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable as V
import Data.Word             (Word8)

import DCF.Transport.Frame   (encodeFrame, DeModFrame)

-- ── Config ─────────────────────────────────────────────────────────────────────

data SymbolConfig = SymbolConfig
  { symSampleRate  :: Double   -- ^ DSP sample rate (Hz)
  , symSymbolRate  :: Double   -- ^ symbol rate (baud)
  } deriving (Show, Eq)

defaultSymbolConfig :: SymbolConfig
defaultSymbolConfig = SymbolConfig
  { symSampleRate = 2_000_000
  , symSymbolRate = 9_600
  }

-- | Samples per symbol (floor of ratio).
samplesPerSymbol :: SymbolConfig -> Int
samplesPerSymbol cfg = floor (symSampleRate cfg / symSymbolRate cfg)

-- | Total samples for a full 17-byte (136-bit) frame.
samplesPerFrame :: SymbolConfig -> Int
samplesPerFrame cfg = 136 * samplesPerSymbol cfg

-- ── Bit extraction ────────────────────────────────────────────────────────────

-- | Extract 8 bits from a byte, MSB first.
--   Returns [(bit_value :: Bool)]
byteToBits :: Word8 -> [Bool]
byteToBits b = [ testBit b (7 - i) | i <- [0..7] ]

-- | Encode a Bool as a BPSK symbol (+1.0 or -1.0).
bpskSymbol :: Bool -> Float
bpskSymbol True  =  1.0
bpskSymbol False = -1.0

-- ── Encoding ─────────────────────────────────────────────────────────────────

-- | Encode a ByteString as a BPSK NRZ symbol stream, upsampled by sps.
--   Output length = 8 * BS.length bs * sps
encodeBpsk :: SymbolConfig -> ByteString -> Vector Float
encodeBpsk cfg bs =
  let sps    = samplesPerSymbol cfg
      bits   = concatMap byteToBits (BS.unpack bs)
      syms   = map bpskSymbol bits
  in V.fromList [ sym | sym <- syms, _ <- [1..sps] ]
     -- Haskell list comprehension: repeat each symbol sps times
     -- Rewrite as explicit concat for clarity:
     -- V.fromList $ concatMap (\s -> replicate sps s) syms

-- | Convenience: encode a sealed DeModFrame directly.
frameToSymbols :: SymbolConfig -> DeModFrame -> Vector Float
frameToSymbols cfg frame = encodeBpsk cfg (encodeFrame frame)

-- ── Notes for future work ─────────────────────────────────────────────────────
-- For QPSK: take pairs of bits, encode into (I, Q) pairs
-- For differential encoding: XOR each bit with the previous before mapping
-- For Gray coding: apply Gray code before the constellation map
-- These are pure functions — perfect for Haskell property-based testing with QuickCheck
