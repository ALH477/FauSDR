-- test/MusicSpec.hs — MIDI and DMX codec tests
-- DeMoD LLC

module MusicSpec (spec) where

import Data.Word             (Word16)
import Test.Hspec
import Test.QuickCheck

import DCF.Transport.Frame
import DCF.Music.MIDI
import DCF.Music.MIDI.Types
import DCF.Music.DMX
import DCF.Sim.Channel
import DCF.Sim.Metrics
import DCF.Sim.Runner

-- ── Arbitrary ────────────────────────────────────────────────────────────────

instance Arbitrary MidiChannel where
  arbitrary = MidiChannel <$> choose (0, 15)

instance Arbitrary MidiNote     where arbitrary = MidiNote     <$> choose (0, 127)
instance Arbitrary MidiVelocity where arbitrary = MidiVelocity <$> choose (0, 127)
instance Arbitrary MidiCC       where arbitrary = MidiCC       <$> choose (0, 127)
instance Arbitrary MidiValue    where arbitrary = MidiValue    <$> choose (0, 127)
instance Arbitrary MidiProgram  where arbitrary = MidiProgram  <$> choose (0, 127)
instance Arbitrary MidiBend     where arbitrary = MidiBend     <$> choose (-8192, 8191)

instance Arbitrary MidiMsg where
  arbitrary = oneof
    [ NoteOff       <$> arbitrary <*> arbitrary <*> arbitrary
    , NoteOn        <$> arbitrary <*> arbitrary <*> arbitrary
    , PolyPressure  <$> arbitrary <*> arbitrary <*> arbitrary
    , ControlChange <$> arbitrary <*> arbitrary <*> arbitrary
    , ProgramChange <$> arbitrary <*> arbitrary
    , ChanPressure  <$> arbitrary <*> arbitrary
    , PitchBend     <$> arbitrary <*> arbitrary
    , pure TimingClock
    , pure MidiStart
    , pure MidiContinue
    , pure MidiStop
    , pure ActiveSensing
    , SongPosition  <$> choose (0, 16383)
    , SongSelect    <$> choose (0, 127)
    , pure TuneRequest
    ]

instance Arbitrary DmxChannel where arbitrary = DmxChannel <$> choose (0, 511)
instance Arbitrary DmxValue   where arbitrary = DmxValue   <$> arbitrary
instance Arbitrary DmxFlags   where arbitrary = DmxFlags   <$> arbitrary <*> arbitrary

-- ── Spec ─────────────────────────────────────────────────────────────────────

spec :: Spec
spec = do

  describe "MIDI codec" $ do

    it "NoteOn round-trips"    $ unpackMidi (packMidi (NoteOn  ch1 n60 vel100)) `shouldBe` Just (NoteOn  ch1 n60 vel100)
    it "NoteOff round-trips"   $ unpackMidi (packMidi (NoteOff ch4 n48 vel0  )) `shouldBe` Just (NoteOff ch4 n48 vel0  )
    it "ControlChange round-trips" $ unpackMidi (packMidi (ControlChange ch10 (MidiCC 7) (MidiValue 127))) `shouldBe`
                                       Just (ControlChange ch10 (MidiCC 7) (MidiValue 127))
    it "ProgramChange round-trips" $ unpackMidi (packMidi (ProgramChange ch1 (MidiProgram 42))) `shouldBe`
                                       Just (ProgramChange ch1 (MidiProgram 42))
    it "PitchBend centre (0) round-trips"  $ unpackMidi (packMidi (PitchBend ch1 (MidiBend 0)))     `shouldBe` Just (PitchBend ch1 (MidiBend 0))
    it "PitchBend +8191 round-trips"       $ unpackMidi (packMidi (PitchBend ch1 (MidiBend 8191)))  `shouldBe` Just (PitchBend ch1 (MidiBend 8191))
    it "PitchBend -8192 round-trips"       $ unpackMidi (packMidi (PitchBend ch1 (MidiBend (-8192)))) `shouldBe` Just (PitchBend ch1 (MidiBend (-8192)))
    it "TimingClock round-trips"  $ unpackMidi (packMidi TimingClock)  `shouldBe` Just TimingClock
    it "MidiStart round-trips"    $ unpackMidi (packMidi MidiStart)    `shouldBe` Just MidiStart
    it "MidiStop round-trips"     $ unpackMidi (packMidi MidiStop)     `shouldBe` Just MidiStop
    it "SongPosition round-trips" $ unpackMidi (packMidi (SongPosition 0x1ABC)) `shouldBe` Just (SongPosition 0x1ABC)
    it "SongSelect round-trips"   $ unpackMidi (packMidi (SongSelect 99))       `shouldBe` Just (SongSelect 99)
    it "TuneRequest round-trips"  $ unpackMidi (packMidi TuneRequest)           `shouldBe` Just TuneRequest

    it "all 16 MIDI channels round-trip" $
      mapM_ (\c ->
        let msg = NoteOn (MidiChannel c) n60 vel100
        in unpackMidi (packMidi msg) `shouldBe` Just msg)
      [0..15]

    it "midiFrame produces valid CRC" $ do
      let frame = midiFrame 0x0001 0xFFFF 42 0 (NoteOn ch1 n60 vel100)
      decodeFrame (encodeFrame frame) `shouldBe` Just frame

    it "extractMidi round-trips through frame" $ do
      let msg   = ControlChange ch1 (MidiCC 1) (MidiValue 64)
          frame = midiFrame 1 2 0 0 msg
      extractMidi frame `shouldBe` Just msg

    it "QC: all MidiMsg constructors round-trip" $
      property $ \msg ->
        unpackMidi (packMidi msg) === Just msg

    it "QC: midiFrame always produces valid CRC" $
      property $ \msg (src :: Word16) (dst :: Word16) (sq :: Word16) ->
        let frame = midiFrame src dst sq 0 msg
        in decodeFrame (encodeFrame frame) === Just frame

  describe "DMX codec" $ do

    it "sparse update pack/unpack round-trips" $
      let u = DmxUpdate (DmxChannel 255) (DmxValue 128) (DmxFlags True False)
      in unpackSparse (packSparse u) `shouldBe` Just u

    it "channel 0 and 511 round-trip" $ do
      let u0   = DmxUpdate (DmxChannel 0)   (DmxValue 0)   defaultFlags
          u511 = DmxUpdate (DmxChannel 511) (DmxValue 255) defaultFlags
      unpackSparse (packSparse u0)   `shouldBe` Just u0
      unpackSparse (packSparse u511) `shouldBe` Just u511

    it "rejects channel > 511" $
      unpackSparse (0x02, 0x00, 0xFF, 0x00) `shouldBe` Nothing

    it "priority flag round-trips" $
      let u = DmxUpdate (DmxChannel 10) (DmxValue 100) (DmxFlags True True)
      in unpackSparse (packSparse u) `shouldBe` Just u

    it "dmxFrame produces valid CRC" $ do
      let frame = dmxFrame 1 2 0 0 (DmxUpdate (DmxChannel 42) (DmxValue 255) defaultFlags)
      decodeFrame (encodeFrame frame) `shouldBe` Just frame

    it "dirtyChannels detects single change" $ do
      let u1    = emptyUniverse
          u2    = setChannel (DmxChannel 5) (DmxValue 200) u1
          dirty = dirtyChannels u1 u2
      length dirty `shouldBe` 1
      dmxChannel (head dirty) `shouldBe` DmxChannel 5
      dmxValue   (head dirty) `shouldBe` DmxValue 200

    it "dirtyChannels is empty for identical universes" $
      dirtyChannels emptyUniverse emptyUniverse `shouldBe` []

    it "setChannel/getChannel round-trips" $ do
      let u = setChannel (DmxChannel 100) (DmxValue 77) emptyUniverse
      getChannel (DmxChannel 100) u `shouldBe` DmxValue 77

    it "universe sync: 256 frames reassemble correctly" $ do
      let uni = foldr (\i u -> setChannel (DmxChannel i)
                                          (DmxValue (fromIntegral i `mod` 256)) u)
                      emptyUniverse [0..255]
          fs  = universeSyncFrames 1 2 0 uni
      length fs `shouldBe` 256
      reassembleUniverse fs `shouldBe` Just uni

    it "universe sync fails on incomplete set" $
      reassembleUniverse (take 200 (universeSyncFrames 1 2 0 emptyUniverse))
        `shouldBe` Nothing

    it "QC: sparse update round-trips for valid channels" $
      property $ \(DmxChannel ch) val flags ->
        let ch' = DmxChannel (ch `mod` 512)
            u   = DmxUpdate ch' val flags
        in unpackSparse (packSparse u) === Just u

  describe "Simulation (thresholded)" $ do

    let cfg = defaultScenarioConfig
                { scFrameCount = 150
                , scIntervalUs = 5000
                , scChannel    = studioConfig
                }

    it "MIDI in studio channel: delivery >= 99%, p99 latency <= 10 ms" $ do
      rep <- runMidiScenario cfg
      deliveryOk 99.0 rep `shouldBe` True
      latencyOk 10_000 rep `shouldBe` True

    it "DMX in studio channel: delivery >= 99%" $ do
      rep <- runDmxScenario cfg
      deliveryOk 99.0 rep `shouldBe` True

    it "Audio clock in stage channel: delivery >= 94%" $ do
      rep <- runAudioClockScenario cfg { scChannel = stageConfig }
      deliveryOk 94.0 rep `shouldBe` True

    it "Latency benchmark in studio: p99 <= 5 ms" $ do
      rep <- runLatencyBenchmark cfg
      latencyOk 5_000 rep `shouldBe` True

-- ── Fixtures ─────────────────────────────────────────────────────────────────

ch1, ch4, ch10 :: MidiChannel
ch1  = MidiChannel 0
ch4  = MidiChannel 3
ch10 = MidiChannel 9

n60, n48 :: MidiNote
n60 = MidiNote 60
n48 = MidiNote 48

vel100, vel0 :: MidiVelocity
vel100 = MidiVelocity 100
vel0   = MidiVelocity 0

defaultFlags :: DmxFlags
defaultFlags = DmxFlags False False
