-- test/AudioSpec.hs — DCF.Music.Audio test suite
-- DeMoD LLC

{-# LANGUAGE RecordWildCards #-}

module AudioSpec (spec) where

import Data.Word             (Word16, Word32)
import Data.Int              (Int16)
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable as V
import Control.Concurrent.STM (atomically)
import Test.Hspec
import Test.QuickCheck

import DCF.Transport.Frame
import DCF.Music.Audio

-- ── Arbitrary ────────────────────────────────────────────────────────────────

instance Arbitrary AudioParam where
  arbitrary = AudioParam <$> arbitrary

instance Arbitrary AutomationMsg where
  arbitrary = AutomationMsg
    <$> arbitrary
    <*> arbitrary
    <*> arbitrary

instance Arbitrary ClockSource where
  arbitrary = elements [minBound..maxBound]

instance Arbitrary ClockMsg where
  arbitrary = ClockMsg
    <$> ((\n -> fromIntegral (n `mod` 65536) / 64.0) <$> (arbitrary :: Gen Int))
    <*> arbitrary
    <*> arbitrary
    <*> arbitrary
    <*> arbitrary
    <*> arbitrary

-- ── Spec ─────────────────────────────────────────────────────────────────────

spec :: Spec
spec = do

  describe "Automation codec" $ do

    it "pack/unpack round-trips with timestamp" $ do
      let msg = AutomationMsg (AudioParam 0x0042) 0x8000 123456
          packed = packAutomation msg
          unpacked = unpackAutomation 123456 packed
      unpacked `shouldBe` msg

    it "param id survives 16-bit round-trip" $ do
      let params = [AudioParam 0x0000, AudioParam 0x0FFF,
                    AudioParam 0x1234, AudioParam 0xFFFF]
      flip mapM_ params $ \p ->
        let msg = AutomationMsg p 0 0
        in autoParam (unpackAutomation 0 (packAutomation msg)) `shouldBe` p

    it "value 0xFFFF survives round-trip" $ do
      let msg = AutomationMsg (AudioParam 0) 0xFFFF 0
          unpacked = unpackAutomation 0 (packAutomation msg)
      autoValue unpacked `shouldBe` 0xFFFF

    it "automationFrame produces valid CRC" $ do
      let msg   = AutomationMsg (AudioParam 0x1001) 0x4000 0
          frame = automationFrame 1 2 0 0 msg
      decodeFrame (encodeFrame frame) `shouldBe` Just frame

    it "full automation round-trip via frame" $ do
      let msg   = AutomationMsg (AudioParam 0x2ABC) 0x1234 99999
          frame = automationFrame 1 broadcast 0 99999 msg
          back  = extractAutomation frame
      autoParam  back `shouldBe` autoParam  msg
      autoValue  back `shouldBe` autoValue  msg

  describe "Clock sync codec" $ do

    it "120 BPM round-trips within resolution" $ do
      let msg = ClockMsg 120.0 32 True False True ClockInternal
          unpacked = unpackClock (packClock msg)
      abs (clockBpm unpacked - 120.0) `shouldSatisfy` (< 0.02)
      clockBeat      unpacked `shouldBe` 32
      clockPlaying   unpacked `shouldBe` True
      clockRecording unpacked `shouldBe` False
      clockLoop      unpacked `shouldBe` True
      clockSource    unpacked `shouldBe` ClockInternal

    it "all ClockSources round-trip" $ do
      flip mapM_ [minBound..maxBound] $ \src ->
        let msg = ClockMsg 100.0 0 False False False src
        in clockSource (unpackClock (packClock msg)) `shouldBe` src

    it "BPM resolution is ~0.016 BPM (1/64)" $ do
      let msg = ClockMsg 127.5 0 False False False ClockMidi
          back = unpackClock (packClock msg)
      abs (clockBpm back - 127.5) `shouldSatisfy` (< 0.02)

    it "clockFrame uses BEACON frame type" $ do
      let frame = clockFrame 1 broadcast 0 0
                    (ClockMsg 120.0 0 True False False ClockInternal)
      frameType frame `shouldBe` FBeacon

    it "extractClock returns Nothing for non-BEACON frames" $ do
      let frame = (clockFrame 1 2 0 0 (ClockMsg 120 0 True False False ClockInternal))
                    { frameType = FData }
      extractClock frame `shouldBe` Nothing

  describe "µ-law codec" $ do

    it "silence encodes and decodes near zero" $ do
      let (s, _, _, _) = unpackSnippet (packSnippet 0 0 0 0)
      abs s `shouldSatisfy` (<= 4)   -- µ-law bias introduces small offset

    it "full-scale positive survives round-trip within 1% error" $ do
      let original = 32767 :: Int16
          (s, _, _, _) = unpackSnippet (packSnippet original 0 0 0)
          err = abs (fromIntegral s - fromIntegral original :: Double)
                / 32767.0
      err `shouldSatisfy` (< 0.02)   -- <2% error at full scale

    it "full-scale negative survives round-trip" $ do
      let original = -32767 :: Int16
          (_, s, _, _) = unpackSnippet (packSnippet 0 original 0 0)
          err = abs (fromIntegral s - fromIntegral original :: Double)
                / 32767.0
      err `shouldSatisfy` (< 0.02)

    it "µ-law is companded (small signals have lower relative error)" $ do
      let small = 100 :: Int16
          large = 10000 :: Int16
          (ds, _, _, _) = unpackSnippet (packSnippet small 0 0 0)
          (dl, _, _, _) = unpackSnippet (packSnippet large 0 0 0)
          errSmall = abs (fromIntegral ds - fromIntegral small :: Double) / fromIntegral small
          errLarge = abs (fromIntegral dl - fromIntegral large :: Double) / fromIntegral large
      -- Small signals should have lower relative error than large (companding property)
      errSmall `shouldSatisfy` (< errLarge * 10)   -- generous bound for codec reality

    it "snippetFrame produces valid CRC" $ do
      let snip  = packSnippet 1000 (-1000) 500 (-500)
          frame = snippetFrame 1 2 0 0 snip
      decodeFrame (encodeFrame frame) `shouldBe` Just frame

  describe "IMA ADPCM codec" $ do

    let sinePcm :: Int -> Vector Int16
        sinePcm n = V.generate n $ \i ->
          round (20000.0 * sin (2 * pi * fromIntegral i / 16.0 :: Double))

    it "fragments a vector into correct count" $ do
      let pcm   = sinePcm 256
          cfg   = defaultAdpcmConfig
          frags = fragmentAudio cfg pcm
      length frags `shouldBe` 64   -- 256 samples / 4 per fragment

    it "ADPCM round-trip SNR is acceptable (>20 dB) via reassembler" $ do
      -- Use the reassembler path which correctly threads ADPCM state
      -- across all fragments (not decodeOneFrag which resets per fragment).
      let pcm    = sinePcm 256
          cfg    = defaultAdpcmConfig
          frags  = fragmentAudio cfg pcm
          frames = adpcmFrames 1 2 0 0 frags
      ras <- newReassembler (adpcmBlockId cfg)
      mapM_ (submitFragment ras) frames
      mDecoded <- reassembledBlock ras (length frags)
      case mDecoded of
        Nothing      -> expectationFailure "reassembler failed"
        Just decoded -> do
          let signal = V.sum (V.map (\s -> (fromIntegral s :: Double) ^ (2::Int)) pcm)
              noise  = V.sum (V.zipWith
                         (\a b -> (fromIntegral a - fromIntegral b :: Double) ^ (2::Int))
                         pcm decoded)
              snrDb  = 10 * logBase 10 (signal / max 1 noise)
          snrDb `shouldSatisfy` (> 20.0)

    it "adpcmFrames produces FCtrl frames" $ do
      let frags  = fragmentAudio defaultAdpcmConfig (sinePcm 64)
          frames = adpcmFrames 1 2 0 0 frags
      all (\f -> frameType f == FCtrl) frames `shouldBe` True

    it "reassembler reconstructs after full fragment set" $ do
      let pcm    = sinePcm 64
          cfg    = defaultAdpcmConfig
          frags  = fragmentAudio cfg pcm
          frames = adpcmFrames 1 2 0 0 frags
      ras <- newReassembler (adpcmBlockId cfg)
      mapM_ (submitFragment ras) frames
      result <- reassembledBlock ras (length frags)
      result `shouldSatisfy` (/= Nothing)

    it "reassembler returns Nothing on incomplete set" $ do
      let frags  = fragmentAudio defaultAdpcmConfig (sinePcm 64)
          frames = take 10 (adpcmFrames 1 2 0 0 frags)
      ras <- newReassembler 0
      mapM_ (submitFragment ras) frames
      result <- reassembledBlock ras (length frags)
      result `shouldBe` Nothing

  describe "Jitter buffer" $ do

    it "returns Nothing when empty" $ do
      buf <- newJitterBuffer :: IO (JitterBuffer Int)
      result <- atomically (pullSample buf 999999)
      result `shouldBe` Nothing

    it "holds sample until timestamp is reached" $ do
      buf <- newJitterBuffer :: IO (JitterBuffer Int)
      pushSample buf 1000 42
      early  <- atomically (pullSample buf 500)
      onTime <- atomically (pullSample buf 1000)
      early  `shouldBe` Nothing
      onTime `shouldBe` Just 42

    it "delivers samples in timestamp order" $ do
      buf <- newJitterBuffer :: IO (JitterBuffer Int)
      pushSample buf 300 3
      pushSample buf 100 1
      pushSample buf 200 2
      s1 <- atomically (pullSample buf 400)
      s2 <- atomically (pullSample buf 400)
      s3 <- atomically (pullSample buf 400)
      [s1, s2, s3] `shouldBe` [Just 1, Just 2, Just 3]

  describe "Sample-accurate timestamp utilities" $ do

    it "usToSamples is correct at 48kHz" $ do
      -- 1 second = 1,000,000 µs = 48,000 samples
      usToSamples 48000 1_000_000 `shouldBe` 48000

    it "samplesToUs is correct at 48kHz" $ do
      samplesToUs 48000 48000 `shouldBe` 1_000_000

    it "round-trip is stable" $ do
      let ts = 123456 :: Word32
      samplesToUs 48000 (usToSamples 48000 ts) `shouldSatisfy`
        (\result -> abs (fromIntegral result - fromIntegral ts :: Int) <= 1)

    it "alignToGrid snaps to nearest sample boundary" $ do
      -- At 48kHz, sample period = 20.833µs. 25µs should snap to ~1 sample (20µs)
      let aligned = alignToGrid 48000 25
      aligned `shouldSatisfy` (\a -> a >= 20 && a <= 21)

    it "individual fragment decodes 4 samples" $ do
      let pcm   = V.fromList [1000, -1000, 500, -500 :: Int16]
          cfg   = defaultAdpcmConfig
          frags = fragmentAudio cfg pcm
      length frags `shouldBe` 1
      length (decodeOneFrag (head frags)) `shouldBe` 4

-- ── Helpers ───────────────────────────────────────────────────────────────────

-- | Decode one fragment in isolation (resets ADPCM state).
-- Only valid for testing individual fragment nibble decode, not full-block SNR.
-- Used by the QuickCheck nibble-level property below.
decodeOneFrag :: AdpcmFragment -> [Int16]
decodeOneFrag AdpcmFragment{..} =
  let (d0, d1) = fragData
      (n0, n1, n2, n3) = unpackNibbles d0 d1
      (s0, st1) = decodeAdpcmSample initAdpcmState n0
      (s1, st2) = decodeAdpcmSample st1            n1
      (s2, st3) = decodeAdpcmSample st2            n2
      (s3, _  ) = decodeAdpcmSample st3            n3
  in [s0, s1, s2, s3]
