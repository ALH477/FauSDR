-- app/Sim.hs — DeMoD RF protocol simulation harness
-- DeMoD LLC
--
-- Usage: cabal run demod-sim [-- --help]
--        cabal run demod-sim [-- --quick] [--channel studio|stage|noisy]

module Main where

import System.Environment  (getArgs)
import System.Exit         (exitSuccess)
import Data.Time.Clock.POSIX (getPOSIXTime)
import Text.Printf         (printf)

import DCF.Sim.Channel
import DCF.Sim.Metrics
import DCF.Sim.Runner

-- ── CLI ───────────────────────────────────────────────────────────────────────

data Opts = Opts
  { optQuick   :: Bool
  , optChannel :: Maybe String   -- Nothing = run all three
  } deriving (Show)

defaultOpts :: Opts
defaultOpts = Opts False Nothing

parseArgs :: [String] -> Either String Opts
parseArgs []                    = Right defaultOpts
parseArgs ("--help"   : _)      = Left helpText
parseArgs ("--quick"  : rest)   = (\o -> o { optQuick = True })   <$> parseArgs rest
parseArgs ("--channel": v: rest) = (\o -> o { optChannel = Just v }) <$> parseArgs rest
parseArgs (flag : _)            = Left $ "Unknown flag: " <> flag <> "\n\n" <> helpText

helpText :: String
helpText = unlines
  [ "demod-sim — DeMoD 900 MHz §15.249 RF protocol simulation"
  , ""
  , "Usage: cabal run demod-sim [-- OPTIONS]"
  , ""
  , "Options:"
  , "  --help              Show this help"
  , "  --quick             200 frames per scenario (faster)"
  , "  --channel NAME      Run one channel: studio | stage | noisy"
  , ""
  , "Scenarios per channel:"
  , "  MIDI          C-major note-on/off stream"
  , "  DMX sparse    16-channel sinusoidal lighting animation"
  , "  Audio clock   120 BPM clock sync frames"
  , "  Latency       Burst mode TimingClock frames"
  , ""
  , "Pass thresholds:"
  , "  Delivery >= 90%  |  p99 latency <= 50 ms"
  ]

-- ── Pass/fail display ─────────────────────────────────────────────────────────

runScenario :: String -> IO SimReport -> IO Bool
runScenario name action = do
  t0  <- getPOSIXTime
  rep <- action
  t1  <- getPOSIXTime
  let elapsed = realToFrac (t1 - t0) :: Double
      ok      = deliveryOk 90.0 rep && latencyOk 50_000 rep
      badge   = if ok then "✓ PASS" else "✗ FAIL"
  printReport (badge <> "  " <> name) rep
  printf "     elapsed: %.2f s\n\n" elapsed
  return ok

-- ── Main ─────────────────────────────────────────────────────────────────────

main :: IO ()
main = do
  args <- getArgs
  opts <- case parseArgs args of
    Left msg -> putStrLn msg >> exitSuccess
    Right o  -> return o

  let frames  = if optQuick opts then 200 else 500
      base    = defaultScenarioConfig { scFrameCount = frames }

  putStrLn ""
  putStrLn "  ╔════════════════════════════════════════════════════╗"
  putStrLn "  ║  DeMoD — RF Protocol Simulation                    ║"
  putStrLn "  ║  900 MHz §15.249 · 57.6k baud · 17-byte frame      ║"
  putStrLn "  ╚════════════════════════════════════════════════════╝"
  putStrLn ""

  let channels =
        [ ("Studio  (0.1% loss, 50µs delay)",    studioConfig)
        , ("Stage   (0.5% loss, 200µs delay)",   stageConfig)
        , ("Venue   (8.0% loss, 1500µs delay)",  noisyConfig)
        ]

  selectedChannels <- case optChannel opts of
    Nothing -> return channels
    Just "studio" -> return [channels !! 0]
    Just "stage"  -> return [channels !! 1]
    Just "noisy"  -> return [channels !! 2]
    Just other    -> do
      putStrLn $ "Unknown channel: " <> other <> " (use studio|stage|noisy)"
      exitSuccess

  results <- mapM (\(label, chCfg) -> do
    putStrLn $ "  ═══ " <> label <> " ═══\n"
    let cfg = base { scChannel = chCfg }
    r1 <- runScenario "MIDI note stream"   (runMidiScenario      cfg)
    r2 <- runScenario "DMX sparse update"  (runDmxScenario       cfg)
    r3 <- runScenario "Audio clock sync"   (runAudioClockScenario cfg)
    r4 <- runScenario "Latency benchmark"  (runLatencyBenchmark   cfg)
    return [r1, r2, r3, r4])
    selectedChannels

  let allPassed = and (concat results)
      total     = length (concat results)
      passed    = length (filter id (concat results))

  putStrLn $ "  ── Summary ─────────────────────────────────────────"
  printf    "     %d/%d scenarios passed\n" passed total
  putStrLn $ if allPassed
               then "     ✓ All scenarios within threshold"
               else "     ✗ Some scenarios out of threshold"
  putStrLn ""
