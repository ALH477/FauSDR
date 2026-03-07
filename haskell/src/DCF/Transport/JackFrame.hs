-- DCF.Transport.JackFrame — DeMoD 8-byte low-latency mini-frame
-- DeMoD LLC | GPL-3.0
--
-- Wire layout (8 bytes = 64 bits):
--   [0]   sync     0xA5
--   [1]   header   [7:4] seq nibble (0-15) | [3:0] type
--   [2-5] payload  4 bytes
--   [6-7] crc16    CRC-CCITT over [0..5]
--
-- Reference vector (DATA, seq=0, payload=0xDEADBEEF):
--   0xA5 0x01 0xDE 0xAD 0xBE 0xEF 0xC2 0x3F
--   CRC pin: 0xC23F
--
-- Latency @ 9600 baud, 96 kHz SR, 32-sample JACK buffer:
--   Frame airtime : 6.667 ms  |  Buffer fill: 0.333 ms
--   One-way total : ~7.0 ms   |  Jitter: ±0.010 ms (±1 sample)

{-# LANGUAGE ScopedTypeVariables #-}

module DCF.Transport.JackFrame
  ( JFType (..)
  , JackFrame (..)
  , encodeJF
  , decodeJF
  , jfSyncByte
  , jfFrameSize
  , jfSetFloat
  , jfGetFloat
  , jfToDCF
  -- * Reference
  , jfReference
  , jfCRCPin
  ) where

import Data.Bits        ((.&.), (.|.), shiftL, shiftR, xor)
import Data.ByteString  (ByteString)
import qualified Data.ByteString as BS
import Data.Word        (Word8, Word16, Word32)
import GHC.Float        (castWord32ToFloat, castFloatToWord32)

import DCF.Transport.Frame (DeModFrame (..), FrameType (..))

-- ── Constants ─────────────────────────────────────────────────────────────────

jfSyncByte :: Word8
jfSyncByte = 0xA5

jfFrameSize :: Int
jfFrameSize = 8

-- ── Types ─────────────────────────────────────────────────────────────────────

data JFType = JFData | JFAck | JFBeacon | JFCtrl
  deriving (Show, Eq, Ord, Enum, Bounded)

data JackFrame = JackFrame
  { jfSeq     :: Word8                          -- ^ 4-bit rolling (0–15)
  , jfType    :: JFType
  , jfPayload :: (Word8, Word8, Word8, Word8)   -- ^ 4 application bytes
  } deriving (Show, Eq)

-- ── CRC-CCITT (poly=0x1021, init=0xFFFF) ─────────────────────────────────────

jfCRC16 :: ByteString -> Word16
jfCRC16 = BS.foldl' step 0xFFFF
  where
    step crc b =
      let crc' = crc `xor` (fromIntegral b `shiftL` 8)
      in  foldl applyBit crc' ([0..7] :: [Int])
    applyBit c _ =
      if c .&. 0x8000 /= 0 then (c `shiftL` 1) `xor` 0x1021
                            else  c `shiftL` 1

-- ── Encode ────────────────────────────────────────────────────────────────────

encodeJF :: JackFrame -> ByteString
encodeJF f =
  let hdr        = ((jfSeq f .&. 0x0F) `shiftL` 4)
                .|. fromIntegral (fromEnum (jfType f) .&. 0x0F)
      (p0,p1,p2,p3) = jfPayload f
      body       = BS.pack [jfSyncByte, hdr, p0, p1, p2, p3]
      crc        = jfCRC16 body
      crcHi      = fromIntegral (crc `shiftR` 8) :: Word8
      crcLo      = fromIntegral (crc .&. 0xFF)   :: Word8
  in  body <> BS.pack [crcHi, crcLo]

-- ── Decode ────────────────────────────────────────────────────────────────────

decodeJF :: ByteString -> Maybe JackFrame
decodeJF bs
  | BS.length bs /= jfFrameSize = Nothing
  | BS.index bs 0 /= jfSyncByte = Nothing
  | jfCRC16 (BS.take 6 bs) /= stored = Nothing
  | otherwise = Just JackFrame
      { jfSeq     = (BS.index bs 1 .&. 0xF0) `shiftR` 4
      , jfType    = toEnum . fromIntegral $ BS.index bs 1 .&. 0x0F
      , jfPayload = ( BS.index bs 2, BS.index bs 3
                    , BS.index bs 4, BS.index bs 5 )
      }
  where
    stored = (fromIntegral (BS.index bs 6) `shiftL` 8)
          .|. fromIntegral (BS.index bs 7) :: Word16

-- ── Float32 helpers ───────────────────────────────────────────────────────────

-- | Pack a Float32 into the payload (big-endian IEEE 754).
jfSetFloat :: Float -> JackFrame -> JackFrame
jfSetFloat v f =
  let w  = castFloatToWord32 v
      b0 = fromIntegral (w `shiftR` 24) :: Word8
      b1 = fromIntegral (w `shiftR` 16) :: Word8
      b2 = fromIntegral (w `shiftR`  8) :: Word8
      b3 = fromIntegral  w              :: Word8
  in  f { jfPayload = (b0, b1, b2, b3) }

-- | Unpack a Float32 from the payload.
jfGetFloat :: JackFrame -> Float
jfGetFloat f =
  let (b0,b1,b2,b3) = jfPayload f
      w = (fromIntegral b0 `shiftL` 24)
       .|. (fromIntegral b1 `shiftL` 16)
       .|. (fromIntegral b2 `shiftL`  8)
       .|.  fromIntegral b3 :: Word32
  in  castWord32ToFloat w

-- ── DCF bridge ────────────────────────────────────────────────────────────────

-- | Wrap JackFrame payload into a full DCF frame for forwarding
--   into the wider DCF network. Caller provides routing metadata.
jfToDCF :: JackFrame -> Word16 -> Word16 -> Word16 -> Word32 -> DeModFrame
jfToDCF jf src dst seqNum ts = DeModFrame
  { frameVersion     = 1
  , frameType        = FData
  , frameSeq         = seqNum
  , frameSrcId       = src
  , frameDstId       = dst
  , framePayload     = jfPayload jf
  , frameTimestampUs = ts
  }

-- ── Reference ─────────────────────────────────────────────────────────────────

-- | Cross-language conformance fixture.
--   DATA, seq=0, payload=0xDEADBEEF.
jfReference :: JackFrame
jfReference = JackFrame
  { jfSeq     = 0
  , jfType    = JFData
  , jfPayload = (0xDE, 0xAD, 0xBE, 0xEF)
  }

-- | encodeJF jfReference must produce bytes [6..7] == 0xC2 0x3F.
jfCRCPin :: Word16
jfCRCPin = 0xC23F
