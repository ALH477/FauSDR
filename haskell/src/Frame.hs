-- DCF.Transport.Frame — DeMoD 17-byte transport frame
-- DeMoD LLC
--
-- Frame wire layout (17 bytes = 136 bits):
--
--  Byte  Field       Notes
--  ────  ─────────   ──────────────────────────────────────────────────
--   0    sync        Fixed 0xD3
--   1    flags       [7:4] version (4-bit) | [3:0] type (4-bit)
--   2-3  seq         Big-endian Word16
--   4-5  src_id      Big-endian Word16
--   6-7  dst_id      Big-endian Word16  (0xFFFF = broadcast)
--   8-11 payload     4 raw bytes
--  12-14 timestamp   24-bit big-endian µs offset (wraps ~16.7 s)
--  15-16 crc16       CRC-CCITT(0x1021, init=0xFFFF) over bytes [0..14]

{-# LANGUAGE ScopedTypeVariables #-}

module DCF.Transport.Frame
  ( FrameType (..)
  , DeModFrame (..)
  , sealFrame
  , validateFrame
  , encodeFrame
  , decodeFrame
  , frameSyncByte
  , broadcast
  ) where

import Data.Bits   ((.&.), (.|.), shiftL, shiftR, xor)
import Data.Word   (Word8, Word16, Word32)
import Data.ByteString (ByteString)
import qualified Data.ByteString      as BS
import qualified Data.ByteString.Lazy as BL
import Data.Binary.Put (Put, putWord8, putWord16be, runPut)

-- ── Constants ─────────────────────────────────────────────────────────────────

frameSyncByte :: Word8
frameSyncByte = 0xD3

broadcast :: Word16
broadcast = 0xFFFF

-- ── Frame type ────────────────────────────────────────────────────────────────

data FrameType
  = FData    -- 0x0 — application data
  | FAck     -- 0x1 — acknowledgement
  | FBeacon  -- 0x2 — clock sync / beacon
  | FCtrl    -- 0x3 — control / fragmented audio
  deriving (Show, Eq, Ord, Enum, Bounded)

frameTypeToNibble :: FrameType -> Word8
frameTypeToNibble = fromIntegral . fromEnum

nibbleToFrameType :: Word8 -> Maybe FrameType
nibbleToFrameType n
  | n <= 3    = Just (toEnum $ fromIntegral n)
  | otherwise = Nothing

-- ── Frame record ─────────────────────────────────────────────────────────────

data DeModFrame = DeModFrame
  { frameVersion     :: Word8                        -- ^ 4-bit (0-15)
  , frameType        :: FrameType
  , frameSeq         :: Word16                       -- ^ rolling counter
  , frameSrcId       :: Word16                       -- ^ source node
  , frameDstId       :: Word16                       -- ^ dest node (0xFFFF = bcast)
  , framePayload     :: (Word8, Word8, Word8, Word8) -- ^ 4 payload bytes
  , frameTimestampUs :: Word32                       -- ^ lower 24 bits used
  } deriving (Show, Eq)

-- ── CRC-CCITT (poly 0x1021, init 0xFFFF) ─────────────────────────────────────

crc16ccitt :: ByteString -> Word16
crc16ccitt = BS.foldl' step 0xFFFF
  where
    step :: Word16 -> Word8 -> Word16
    step crc b =
      let crc' = crc `xor` (fromIntegral b `shiftL` 8)
      in foldl applyBit crc' ([0..7] :: [Int])
    applyBit :: Word16 -> Int -> Word16
    applyBit c _ =
      if c .&. 0x8000 /= 0
        then (c `shiftL` 1) `xor` 0x1021
        else  c `shiftL` 1

-- ── Serialisation ─────────────────────────────────────────────────────────────

putFrameBody :: DeModFrame -> Put
putFrameBody f = do
  putWord8 frameSyncByte
  let flags = ((frameVersion f .&. 0x0F) `shiftL` 4)
            .|. (frameTypeToNibble (frameType f) .&. 0x0F)
  putWord8 flags
  putWord16be (frameSeq   f)
  putWord16be (frameSrcId f)
  putWord16be (frameDstId f)
  let (p0, p1, p2, p3) = framePayload f
  putWord8 p0; putWord8 p1; putWord8 p2; putWord8 p3
  let ts = frameTimestampUs f
  putWord8 (fromIntegral $ (ts `shiftR` 16) .&. 0xFF)
  putWord8 (fromIntegral $ (ts `shiftR`  8) .&. 0xFF)
  putWord8 (fromIntegral $  ts              .&. 0xFF)

-- | Produce the sealed 17-byte frame ByteString.
encodeFrame :: DeModFrame -> ByteString
encodeFrame f =
  let body  = BL.toStrict . runPut $ putFrameBody f
      crc   = crc16ccitt body
      crcHi = fromIntegral (crc `shiftR` 8) :: Word8
      crcLo = fromIntegral (crc .&. 0xFF)   :: Word8
  in body <> BS.pack [crcHi, crcLo]

-- | Validate and decode a 17-byte ByteString.
--   Returns Nothing if the sync byte is wrong or CRC fails.
decodeFrame :: ByteString -> Maybe DeModFrame
decodeFrame bs
  | BS.length bs /= 17             = Nothing
  | BS.index bs 0 /= frameSyncByte = Nothing
  | otherwise =
      let body      = BS.take 15 bs
          crcStored = (fromIntegral (BS.index bs 15) `shiftL` 8)
                    .|. fromIntegral (BS.index bs 16) :: Word16
          crcCalc   = crc16ccitt body
      in if crcCalc /= crcStored
           then Nothing
           else
             let flags   = BS.index bs 1
                 ver     = (flags `shiftR` 4) .&. 0x0F
                 typeNib = flags .&. 0x0F
                 ftype   = case nibbleToFrameType typeNib of
                              Just t  -> t
                              Nothing -> FData
                 seqNum  = (fromIntegral (BS.index bs 2) `shiftL` 8)
                         .|. fromIntegral (BS.index bs 3)
                 srcId   = (fromIntegral (BS.index bs 4) `shiftL` 8)
                         .|. fromIntegral (BS.index bs 5)
                 dstId   = (fromIntegral (BS.index bs 6) `shiftL` 8)
                         .|. fromIntegral (BS.index bs 7)
                 payload = ( BS.index bs  8, BS.index bs  9
                           , BS.index bs 10, BS.index bs 11 )
                 ts      = (fromIntegral (BS.index bs 12) `shiftL` 16)
                         .|. (fromIntegral (BS.index bs 13) `shiftL` 8)
                         .|.  fromIntegral (BS.index bs 14)
             in Just DeModFrame
                  { frameVersion     = ver
                  , frameType        = ftype
                  , frameSeq         = seqNum
                  , frameSrcId       = srcId
                  , frameDstId       = dstId
                  , framePayload     = payload
                  , frameTimestampUs = ts
                  }

-- ── Convenience aliases ───────────────────────────────────────────────────────

sealFrame :: DeModFrame -> ByteString
sealFrame = encodeFrame

validateFrame :: ByteString -> Maybe DeModFrame
validateFrame = decodeFrame
