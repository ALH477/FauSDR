-- test/FrameSpec.hs — Frame + Symbol encoder test suite
-- DeMoD LLC
--
-- Run standalone:  cabal test frame-tests
-- Run via Spec.hs: imported as FrameSpec.spec

module FrameSpec (spec) where

import Data.ByteString       (ByteString)
import qualified Data.ByteString as BS
import Data.Word             (Word8, Word16)
import Data.Bits             (xor)
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable as V
import Test.Hspec
import Test.QuickCheck

import DCF.Transport.Frame
import DCF.Transport.Symbol

-- ── Arbitrary instances ───────────────────────────────────────────────────────

instance Arbitrary FrameType where
  arbitrary = elements [minBound .. maxBound]

instance Arbitrary DeModFrame where
  arbitrary = DeModFrame
    <$> ((`mod` 16) <$> arbitrary)
    <*> arbitrary
    <*> arbitrary
    <*> arbitrary
    <*> arbitrary
    <*> ((,,,) <$> arbitrary <*> arbitrary <*> arbitrary <*> arbitrary)
    <*> ((`mod` 0xFFFFFF) <$> arbitrary)

-- ── Spec ─────────────────────────────────────────────────────────────────────

spec :: Spec
spec = do

  describe "Frame serialisation" $ do

    it "encodeFrame produces exactly 17 bytes" $
      BS.length (encodeFrame exampleFrame) `shouldBe` 17

    it "sync byte is always 0xD3" $
      BS.head (encodeFrame exampleFrame) `shouldBe` 0xD3

    it "broadcast constant is 0xFFFF" $
      broadcast `shouldBe` 0xFFFF

    it "round-trips through decodeFrame" $
      decodeFrame (encodeFrame exampleFrame) `shouldBe` Just exampleFrame

    it "sealFrame and encodeFrame are identical" $
      sealFrame exampleFrame `shouldBe` encodeFrame exampleFrame

    it "validateFrame and decodeFrame are identical" $
      validateFrame (encodeFrame exampleFrame) `shouldBe` decodeFrame (encodeFrame exampleFrame)

    it "rejects 16-byte input" $
      decodeFrame (BS.replicate 16 0x00) `shouldBe` Nothing

    it "rejects 18-byte input" $
      decodeFrame (BS.replicate 18 0x00) `shouldBe` Nothing

    it "rejects empty input" $
      decodeFrame BS.empty `shouldBe` Nothing

    it "rejects wrong sync byte" $ do
      let bs  = encodeFrame exampleFrame
          bad = BS.cons 0x00 (BS.tail bs)
      decodeFrame bad `shouldBe` Nothing

    it "rejects single-bit corruption in payload" $
      decodeFrame (flipByte 8 (encodeFrame exampleFrame)) `shouldBe` Nothing

    it "rejects single-bit corruption in header" $
      decodeFrame (flipByte 2 (encodeFrame exampleFrame)) `shouldBe` Nothing

    it "rejects single-bit corruption in timestamp" $
      decodeFrame (flipByte 12 (encodeFrame exampleFrame)) `shouldBe` Nothing

    it "all FrameType values survive round-trip" $
      mapM_ (\ft ->
        let f = exampleFrame { frameType = ft }
        in decodeFrame (encodeFrame f) `shouldBe` Just f)
      [minBound..maxBound]

    it "version 0–15 all survive round-trip" $
      mapM_ (\v ->
        let f = exampleFrame { frameVersion = v }
        in (frameVersion <$> decodeFrame (encodeFrame f)) `shouldBe` Just v)
      [0..15]

    it "broadcast dst_id survives round-trip" $ do
      let f = exampleFrame { frameDstId = broadcast }
      (frameDstId <$> decodeFrame (encodeFrame f)) `shouldBe` Just broadcast

    it "max timestamp (0xFFFFFF) survives round-trip" $ do
      let f = exampleFrame { frameTimestampUs = 0xFFFFFF }
      (frameTimestampUs <$> decodeFrame (encodeFrame f)) `shouldBe` Just 0xFFFFFF

    it "QuickCheck: round-trip holds for arbitrary frames" $
      property $ \f ->
        decodeFrame (encodeFrame f) === Just f

  describe "CRC properties" $ do

    it "different payloads produce different CRCs" $ do
      let f1 = exampleFrame { framePayload = (0x00,0x00,0x00,0x00) }
          f2 = exampleFrame { framePayload = (0xFF,0xFF,0xFF,0xFF) }
      encodeFrame f1 `shouldNotBe` encodeFrame f2

    it "CRC bytes are in the last two positions" $ do
      let bs  = encodeFrame exampleFrame
          -- flipping CRC bytes themselves doesn't change what we're validating
          bad = flipByte 15 bs
      decodeFrame bad `shouldBe` Nothing

    it "QuickCheck: any single-byte corruption is detected" $
      -- Corrupt any byte in positions 0-14 (body); CRC should catch it.
      -- Position 0 (sync) is caught by sync check before CRC.
      property $ \(f :: DeModFrame) (pos :: Int) ->
        let bs  = encodeFrame f
            p   = (abs pos `mod` 14) + 1   -- positions 1-14 (avoid sync byte)
            bad = flipByte p bs
        in decodeFrame bad === Nothing

  describe "Symbol encoder" $ do

    let cfg = defaultSymbolConfig

    it "encodes 17 bytes into 136 * samplesPerSymbol samples" $ do
      let sps = samplesPerSymbol cfg
      V.length (frameToSymbols cfg exampleFrame) `shouldBe` 136 * sps

    it "produces only +1.0 and -1.0 values" $
      V.all (\x -> x == 1.0 || x == (-1.0))
            (encodeBpsk cfg (BS.replicate 4 0xAA))
        `shouldBe` True

    it "0xAA (10101010) → alternating +1 -1 symbols" $ do
      let sps = samplesPerSymbol cfg
          vec = encodeBpsk cfg (BS.singleton 0xAA)
          syms = map (\i -> vec V.! (i * sps)) [0..7]
      syms `shouldBe` [1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0]

    it "0x00 → all -1.0" $
      V.all (== (-1.0)) (encodeBpsk cfg (BS.singleton 0x00)) `shouldBe` True

    it "0xFF → all +1.0" $
      V.all (== 1.0) (encodeBpsk cfg (BS.singleton 0xFF)) `shouldBe` True

    it "samplesPerFrame = 136 * samplesPerSymbol" $
      samplesPerFrame cfg `shouldBe` 136 * samplesPerSymbol cfg

-- ── Fixtures and helpers ──────────────────────────────────────────────────────

exampleFrame :: DeModFrame
exampleFrame = DeModFrame
  { frameVersion     = 1
  , frameType        = FData
  , frameSeq         = 0x1234
  , frameSrcId       = 0x0001
  , frameDstId       = 0xFFFF
  , framePayload     = (0xDE, 0xAD, 0xBE, 0xEF)
  , frameTimestampUs = 0xAB12CD
  }

-- | Flip all bits in the nth byte of a ByteString.
flipByte :: Int -> ByteString -> ByteString
flipByte i bs =
  let (pre, rest) = BS.splitAt i bs
  in case BS.uncons rest of
       Nothing      -> bs
       Just (b, tl) -> pre <> BS.singleton (xor b 0xFF) <> tl
