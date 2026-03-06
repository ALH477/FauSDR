-- test/ConformanceSpec.hs — Cross-language codec conformance (Haskell side)
-- DeMoD LLC
--
-- This spec asserts the canonical reference test vector and CRC pin 0x42DD.
-- The same vector is pinned in:
--   C    — src/test_conformance.cpp  (crc_pin, reference_vector_encode)
--   Rust — dcf/rust/src/frame.rs     (#[test] crc_cross_language_pin)
--   Lisp — hydramesh.lisp            (dcf-frame-crc-test)
--
-- If ANY language's test fails the pin, that codec has diverged.

module ConformanceSpec (spec) where

import Data.ByteString (ByteString)
import qualified Data.ByteString as BS
import Data.Word       (Word8)
import Test.Hspec

import DCF.Transport.Frame

spec :: Spec
spec = do

  describe "Cross-language conformance" $ do

    -- ── Reference test vector ─────────────────────────────────────────────────
    -- version=1, DATA, seq=1, src=1, dst=0xFFFF, payload=DEADBEEF, ts=0
    -- Expected: D3 10 00 01 00 01 FF FF DE AD BE EF 00 00 00 42 DD

    let referenceFrame = DeModFrame
          { frameVersion     = 1
          , frameType        = FData
          , frameSeq         = 1
          , frameSrcId       = 0x0001
          , frameDstId       = 0xFFFF
          , framePayload     = (0xDE, 0xAD, 0xBE, 0xEF)
          , frameTimestampUs = 0
          }

        referenceWire :: ByteString
        referenceWire = BS.pack
          [ 0xD3, 0x10, 0x00, 0x01
          , 0x00, 0x01, 0xFF, 0xFF
          , 0xDE, 0xAD, 0xBE, 0xEF
          , 0x00, 0x00, 0x00
          , 0x42, 0xDD             -- CRC pin
          ]

    it "encodes reference frame to exact 17-byte sequence" $
      encodeFrame referenceFrame `shouldBe` referenceWire

    it "CRC pin: last two bytes are 0x42 0xDD" $ do
      let wire = encodeFrame referenceFrame
      BS.index wire 15 `shouldBe` 0x42
      BS.index wire 16 `shouldBe` 0xDD

    it "decodes reference wire to reference frame" $
      decodeFrame referenceWire `shouldBe` Just referenceFrame

    it "sync byte is 0xD3" $
      BS.index referenceWire 0 `shouldBe` 0xD3

    it "flags byte encodes version=1, type=DATA as 0x10" $
      BS.index referenceWire 1 `shouldBe` 0x10

    it "seq big-endian: bytes [2,3] = [0x00, 0x01]" $ do
      BS.index referenceWire 2 `shouldBe` 0x00
      BS.index referenceWire 3 `shouldBe` 0x01

    it "src_id big-endian: bytes [4,5] = [0x00, 0x01]" $ do
      BS.index referenceWire 4 `shouldBe` 0x00
      BS.index referenceWire 5 `shouldBe` 0x01

    it "dst_id broadcast: bytes [6,7] = [0xFF, 0xFF]" $ do
      BS.index referenceWire 6 `shouldBe` 0xFF
      BS.index referenceWire 7 `shouldBe` 0xFF

    it "payload bytes [8..11] = DE AD BE EF" $ do
      BS.index referenceWire  8 `shouldBe` 0xDE
      BS.index referenceWire  9 `shouldBe` 0xAD
      BS.index referenceWire 10 `shouldBe` 0xBE
      BS.index referenceWire 11 `shouldBe` 0xEF

    it "timestamp zero: bytes [12,13,14] = [0x00, 0x00, 0x00]" $ do
      BS.index referenceWire 12 `shouldBe` 0x00
      BS.index referenceWire 13 `shouldBe` 0x00
      BS.index referenceWire 14 `shouldBe` 0x00

    it "reference wire is exactly 17 bytes" $
      BS.length referenceWire `shouldBe` 17

    it "reference wire is accepted as valid" $
      decodeFrame referenceWire `shouldNotBe` Nothing
