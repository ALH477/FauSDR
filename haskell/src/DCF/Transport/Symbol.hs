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
-- which Faust's pulse shaping filter convolves with the RRC kernel.
--
-- The full frame (17 bytes = 136 bits) produces a vector of length:
--   136 * samplesPerSymbol
--
-- With sampleRate=2e6 and symbolRate=9600:
--   samplesPerSymbol = 2_000_000 / 9_600 ≈ 208
--
-- BUG FIX: the previous implementation built the output via a Haskell list
-- comprehension ([sym | sym <- syms, _ <- [1..sps]]) which allocated a large
-- intermediate list before converting to Vector.  For a 17-byte frame at 2 MSPS
-- and 9600 baud this is ~28,000 Float cells on the heap per frame — harmless at
-- low frame rates but gratuitous.  Replaced with V.generate which writes
-- directly into the target Vector with no intermediate allocation.

module DCF.Transport.Symbol
  ( SymbolConfig (..)
  , defaultSymbolConfig
  , encodeBpsk
  , frameToSymbols
  , samplesPerFrame
  , samplesPerSymbol
  ) where

import Data.Bits             (testBit)
import Data.ByteString       (ByteString)
import qualified Data.ByteString as BS
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable as V
import Data.Word             (Word8)

import DCF.Transport.Frame   (encodeFrame, DeModFrame)

-- ── Config ────────────────────────────────────────────────────────────────────

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

-- | Extract 8 bits from a byte, MSB first, as BPSK NRZ-L symbols (+1/-1).
byteToSymbols :: Word8 -> [Float]
byteToSymbols b = [ if testBit b (7 - i) then 1.0 else -1.0 | i <- [0..7] ]

-- ── Encoding ─────────────────────────────────────────────────────────────────

-- | Encode a ByteString as a BPSK NRZ symbol stream, upsampled by sps.
--   Output length = 8 * BS.length bs * sps
--
--   Uses V.generate to write directly into the output Vector —
--   no intermediate list allocation.
encodeBpsk :: SymbolConfig -> ByteString -> Vector Float
encodeBpsk cfg bs =
  let sps    = samplesPerSymbol cfg
      -- Build a flat array of per-bit symbols (length = nBits)
      syms   = concatMap byteToSymbols (BS.unpack bs)
      nBits  = length syms
      totalSamples = nBits * sps
  in V.generate totalSamples $ \i ->
       -- Sample i belongs to bit (i `div` sps)
       syms !! (i `div` sps)

-- | Convenience: encode a sealed DeModFrame directly.
frameToSymbols :: SymbolConfig -> DeModFrame -> Vector Float
frameToSymbols cfg frame = encodeBpsk cfg (encodeFrame frame)

-- ── Notes for future modulation schemes ──────────────────────────────────────
-- For QPSK: take pairs of bits → (I, Q) pairs → two separate Vectors
-- For differential BPSK: XOR each bit with the previous before mapping
-- For Gray coding: apply Gray code before the constellation map
-- These are pure functions — ideal for QuickCheck property-based testing
