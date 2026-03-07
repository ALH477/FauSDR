-- test/JackFrameSpec.hs — JackFrame cross-language conformance tests
-- DeMoD LLC | GPL-3.0
--
-- Tests the 8-byte mini-frame codec against the reference vector.
-- CRC pin 0xC23F is the cross-language gate: C, Haskell, Python/GR must agree.

module JackFrameSpec (spec) where

import Test.Hspec
import Test.QuickCheck
import Data.ByteString  (ByteString)
import qualified Data.ByteString as BS
import Data.Word        (Word8)

import DCF.Transport.JackFrame

spec :: Spec
spec = do
  describe "JackFrame codec" $ do

    it "encodes reference frame to exactly 8 bytes" $
      BS.length (encodeJF jfReference) `shouldBe` 8

    it "CRC pin: bytes [6..7] = 0xC2 0x3F" $ do
      let wire = encodeJF jfReference
      BS.index wire 6 `shouldBe` 0xC2
      BS.index wire 7 `shouldBe` 0x3F

    it "sync byte is 0xA5" $
      BS.index (encodeJF jfReference) 0 `shouldBe` 0xA5

    it "payload bytes 2-5 = DE AD BE EF" $ do
      let wire = encodeJF jfReference
      BS.index wire 2 `shouldBe` 0xDE
      BS.index wire 3 `shouldBe` 0xAD
      BS.index wire 4 `shouldBe` 0xBE
      BS.index wire 5 `shouldBe` 0xEF

    it "decodes reference frame correctly" $ do
      let wire = encodeJF jfReference
      decodeJF wire `shouldBe` Just jfReference

    it "rejects wrong sync byte" $
      decodeJF (BS.cons 0x00 (BS.tail (encodeJF jfReference)))
        `shouldBe` Nothing

    it "rejects CRC corruption" $ do
      let wire = encodeJF jfReference
          bad  = BS.init wire <> BS.singleton (BS.last wire `xor` 0xFF)
      decodeJF bad `shouldBe` Nothing

    it "QuickCheck: round-trip holds" $
      property $ \(s, t, p0, p1, p2, p3) ->
        let f    = JackFrame (s `mod` 16) (toEnum (t `mod` 4))
                             (p0, p1, p2, p3)
            wire = encodeJF f
        in decodeJF wire == Just f

    it "jfGetFloat . jfSetFloat = id (approximately)" $
      property $ \(v :: Float) ->
        let f  = JackFrame 0 JFData (0,0,0,0)
            f' = jfSetFloat v f
        in  jfGetFloat f' == v

    it "seq nibble wraps at 16" $ do
      let f    = JackFrame 15 JFData (0,0,0,0)
          wire = encodeJF f
          hdr  = BS.index wire 1
      (hdr `div` 16) `shouldBe` 15
