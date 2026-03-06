-- DCF.Sim.Runner — simulation scenarios for MIDI, DMX, and audio
-- DeMoD LLC
--
-- BUG FIX vs previous: putMVar done () was called from both TX thread (N
-- frames sent) and RX thread (N frames received). takeMVar only consumed
-- one, leaving the second put blocking the thread forever (thread leak).
-- Fixed: TX thread signals via txDone; main waits for rxDone only.
-- RX loop drives completion; TX thread just fires and forgets.

module DCF.Sim.Runner
  ( ScenarioConfig (..)
  , defaultScenarioConfig
  , runMidiScenario
  , runDmxScenario
  , runAudioClockScenario
  , runLatencyBenchmark
  ) where

import Control.Concurrent   (forkIO, threadDelay, MVar, newEmptyMVar, putMVar, takeMVar)
import Control.Monad        (forM_, when, void)
import Data.IORef           (newIORef, readIORef, modifyIORef')
import Data.Time.Clock.POSIX (getPOSIXTime)
import Data.Word            (Word16, Word32)

import DCF.Transport.Frame
import DCF.Music.MIDI
import DCF.Music.DMX
import DCF.Music.Audio
import DCF.Sim.Channel
import DCF.Sim.Metrics

-- ── Config ─────────────────────────────────────────────────────────────────────

data ScenarioConfig = ScenarioConfig
  { scFrameCount :: Int           -- ^ total frames to transmit
  , scIntervalUs :: Int           -- ^ µs between frames (0 = burst)
  , scSrcId      :: Word16
  , scDstId      :: Word16
  , scChannel    :: ChannelConfig
  } deriving (Show)

defaultScenarioConfig :: ScenarioConfig
defaultScenarioConfig = ScenarioConfig
  { scFrameCount = 1000
  , scIntervalUs = 2400      -- 57.6k baud frame period (~2.36 ms)
  , scSrcId      = 0x0001
  , scDstId      = 0x0002
  , scChannel    = stageConfig
  }

-- ── Timestamp ─────────────────────────────────────────────────────────────────

nowUs :: IO Int
nowUs = round . (* 1_000_000) <$> getPOSIXTime

nowUs24 :: IO Word32
nowUs24 = (`mod` 0xFFFFFF) . fromIntegral <$> nowUs

-- ── MIDI scenario ─────────────────────────────────────────────────────────────

runMidiScenario :: ScenarioConfig -> IO SimReport
runMidiScenario cfg = do
  ch  <- newChannel (scChannel cfg)
  met <- newMetrics
  rxDone <- newEmptyMVar

  let ch1   = MidiChannel 0
      notes = cycle [60,62,64,65,67,69,71,72]   -- C major

  -- TX: fire and forget
  void $ forkIO $ do
    seqRef <- newIORef (0 :: Word16)
    forM_ (zip [0..scFrameCount cfg - 1] (zip (zip notes (tail notes))
                                              (cycle [True,False]))) $
      \(_, ((n, _), isOn)) -> do
        sq <- readIORef seqRef
        modifyIORef' seqRef (+1)
        ts <- nowUs24
        let msg = if isOn
                    then NoteOn  ch1 (MidiNote (fromIntegral n)) (MidiVelocity 100)
                    else NoteOff ch1 (MidiNote (fromIntegral n)) (MidiVelocity 0)
        recordTx met
        transmit ch (encodeFrame (midiFrame (scSrcId cfg) (scDstId cfg) sq ts msg))
        when (scIntervalUs cfg > 0) $ threadDelay (scIntervalUs cfg)

  -- RX: signals completion via rxDone
  void $ forkIO $ receiveLoop ch met (scFrameCount cfg) rxDone

  takeMVar rxDone
  threadDelay 60_000   -- let in-flight frames drain
  closeChannel ch
  getReport met

-- ── DMX sparse scenario ───────────────────────────────────────────────────────

runDmxScenario :: ScenarioConfig -> IO SimReport
runDmxScenario cfg = do
  ch  <- newChannel (scChannel cfg)
  met <- newMetrics
  rxDone <- newEmptyMVar

  let movingChans = [0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45]

  void $ forkIO $ do
    seqRef <- newIORef (0 :: Word16)
    forM_ [0..scFrameCount cfg - 1] $ \i -> do
      sq <- readIORef seqRef
      modifyIORef' seqRef (+1)
      ts  <- nowUs24
      let ci    = i `mod` length movingChans
          ch_n  = fromIntegral (movingChans !! ci)
          phase = fromIntegral i * 0.05 :: Double
          val   = round (127.5 + 127.5 * sin (phase + fromIntegral ci)) :: Int
          upd   = DmxUpdate (DmxChannel ch_n)
                            (DmxValue (fromIntegral (val `mod` 256)))
                            defaultFlags
      recordTx met
      transmit ch (encodeFrame (dmxFrame (scSrcId cfg) (scDstId cfg) sq ts upd))
      when (scIntervalUs cfg > 0) $ threadDelay (scIntervalUs cfg)

  void $ forkIO $ receiveLoop ch met (scFrameCount cfg) rxDone
  takeMVar rxDone
  threadDelay 60_000
  closeChannel ch
  getReport met

-- ── Audio clock scenario ───────────────────────────────────────────────────────

runAudioClockScenario :: ScenarioConfig -> IO SimReport
runAudioClockScenario cfg = do
  ch  <- newChannel (scChannel cfg)
  met <- newMetrics
  rxDone <- newEmptyMVar

  void $ forkIO $ do
    seqRef <- newIORef (0 :: Word16)
    forM_ [0..scFrameCount cfg - 1] $ \i -> do
      sq  <- readIORef seqRef
      modifyIORef' seqRef (+1)
      ts  <- nowUs24
      let beat = fromIntegral (i `mod` 256) :: Word8
          msg  = ClockMsg 120.0 beat True False False ClockInternal
      recordTx met
      transmit ch (encodeFrame (clockFrame (scSrcId cfg) (scDstId cfg) sq ts msg))
      when (scIntervalUs cfg > 0) $ threadDelay (scIntervalUs cfg)

  void $ forkIO $ receiveLoop ch met (scFrameCount cfg) rxDone
  takeMVar rxDone
  threadDelay 60_000
  closeChannel ch
  getReport met

-- ── Latency benchmark ─────────────────────────────────────────────────────────

runLatencyBenchmark :: ScenarioConfig -> IO SimReport
runLatencyBenchmark cfg = do
  ch  <- newChannel (scChannel cfg)
  met <- newMetrics
  rxDone <- newEmptyMVar

  void $ forkIO $ do
    seqRef <- newIORef (0 :: Word16)
    forM_ [1..scFrameCount cfg] $ \_ -> do
      sq  <- readIORef seqRef
      modifyIORef' seqRef (+1)
      ts  <- nowUs24
      let frame = midiFrame (scSrcId cfg) (scDstId cfg) sq ts TimingClock
      recordTx met
      transmit ch (encodeFrame frame)

  void $ forkIO $ receiveLoop ch met (scFrameCount cfg) rxDone
  takeMVar rxDone
  threadDelay 100_000
  closeChannel ch
  getReport met

-- ── Shared RX loop ────────────────────────────────────────────────────────────
--
-- Only this loop writes to rxDone. TX thread never touches it.

receiveLoop :: RfChannel -> Metrics -> Int -> MVar () -> IO ()
receiveLoop ch met total done = go 0
  where
    go !n = do
      mBs <- receive ch
      case mBs of
        Nothing -> putMVar done ()   -- channel closed before target reached
        Just bs -> do
          tNow <- nowUs
          case decodeFrame bs of
            Nothing -> do
              recordCrcFail met
              go n
            Just frame -> do
              let tsSent = fromIntegral (frameTimestampUs frame) :: Int
                  lat    = abs ((tNow `mod` 0x1000000) - (tsSent `mod` 0x1000000))
              recordRx met lat
              let n' = n + 1
              if n' >= total
                then putMVar done ()
                else go n'
