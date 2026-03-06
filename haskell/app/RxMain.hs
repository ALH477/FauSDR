-- RxMain.hs — DeMoD Faust-SDR Haskell RX entry point
-- DeMoD LLC
--
-- Usage:
--   demod-rx-hs [--driver rtlsdr] [--freq 433.92e6] [--rate 2e6] [--gain 40]
--               [--count N]
--
-- Receives a stream of DCF frames, prints their contents to stdout.
-- Ctrl+C to stop.

module Main where

import Control.Monad       (when)
import Data.IORef          (newIORef, modifyIORef', readIORef)
import Data.Word           (Word8)
import System.Environment  (getArgs)
import System.Exit         (exitFailure)
import Text.Read           (readMaybe)
import Text.Printf         (printf)

import DCF.Transport.Frame
import DCF.Demodulator

-- ── CLI ──────────────────────────────────────────────────────────────────────

data CLI = CLI
  { cliDriver :: String
  , cliFreq   :: Double
  , cliRate   :: Double
  , cliGain   :: Double
  , cliCount  :: Maybe Int
  } deriving (Show)

defaultCLI :: CLI
defaultCLI = CLI
  { cliDriver = "rtlsdr"
  , cliFreq   = 433.92e6
  , cliRate   = 2e6
  , cliGain   = 40.0
  , cliCount  = Nothing
  }

parseCLI :: [String] -> CLI -> Either String CLI
parseCLI [] cli = Right cli
parseCLI ("--driver" : v : rest) cli = parseCLI rest cli { cliDriver = v }
parseCLI ("--freq"   : v : rest) cli = case readMaybe v of
  Just d  -> parseCLI rest cli { cliFreq = d }
  Nothing -> Left $ "Invalid --freq: " <> v
parseCLI ("--rate"   : v : rest) cli = case readMaybe v of
  Just d  -> parseCLI rest cli { cliRate = d }
  Nothing -> Left $ "Invalid --rate: " <> v
parseCLI ("--gain"   : v : rest) cli = case readMaybe v of
  Just d  -> parseCLI rest cli { cliGain = d }
  Nothing -> Left $ "Invalid --gain: " <> v
parseCLI ("--count"  : v : rest) cli = case readMaybe v of
  Just n  -> parseCLI rest cli { cliCount = Just n }
  Nothing -> Left $ "Invalid --count: " <> v
parseCLI ("--help" : _) _ = Left ""
parseCLI (flag : _) _     = Left $ "Unknown flag: " <> flag

usage :: String
usage = unlines
  [ "Usage: demod-rx-hs [OPTIONS]"
  , ""
  , "  --driver NAME   SoapySDR driver  (default: rtlsdr)"
  , "  --freq   HZ     Centre frequency (default: 433.92e6)"
  , "  --rate   HZ     Sample rate      (default: 2e6)"
  , "  --gain   DB     RX gain          (default: 40)"
  , "  --count  N      Stop after N frames (default: infinite)"
  , "  --help          Show this message"
  ]

-- ── Frame printer ─────────────────────────────────────────────────────────────

printFrame :: Int -> DeModFrame -> IO ()
printFrame n f = do
  let (p0, p1, p2, p3) = framePayload f
  printf "[rx #%04d] ver=%d type=%-6s seq=%05d src=0x%04X dst=0x%04X payload=%02X%02X%02X%02X ts=%d\n"
    n
    (frameVersion f)
    (show (frameType f))
    (frameSeq f)
    (frameSrcId f)
    (frameDstId f)
    (p0 :: Word8) (p1 :: Word8) (p2 :: Word8) (p3 :: Word8)
    (frameTimestampUs f)

-- ── Main ─────────────────────────────────────────────────────────────────────

main :: IO ()
main = do
  args <- getArgs
  cli  <- case parseCLI args defaultCLI of
    Left ""  -> putStr usage >> exitFailure
    Left err -> putStrLn ("Error: " <> err) >> putStr usage >> exitFailure
    Right c  -> return c

  putStrLn "[demod-rx] DeMoD Faust-SDR Haskell RX"
  putStrLn $ "[demod-rx] driver=" <> cliDriver cli
  putStrLn $ "[demod-rx] freq="   <> show (cliFreq cli / 1e6) <> " MHz"
  putStrLn $ "[demod-rx] rate="   <> show (cliRate cli / 1e6) <> " MSPS"
  putStrLn $ "[demod-rx] gain="   <> show (cliGain cli)       <> " dB"
  putStrLn $ "[demod-rx] count="  <> maybe "infinite" show (cliCount cli)

  let cfg = defaultDemodConfig
        { demodSdr = (demodSdr defaultDemodConfig)
            { rxDriver       = cliDriver cli
            , rxCenterFreqHz = cliFreq cli
            , rxSampleRateHz = cliRate cli
            , rxGainDb       = cliGain cli
            }
        , demodDsp = (demodDsp defaultDemodConfig)
            { dspSampleRate = round (cliRate cli) }
        , demodSymbol = (demodSymbol defaultDemodConfig)
            { symSampleRate = cliRate cli }
        }

  countRef <- newIORef (0 :: Int)

  putStrLn "[demod-rx] Acquiring carrier lock..."
  runDemodulator cfg $ \frame -> do
    n <- readIORef countRef
    modifyIORef' countRef (+ 1)
    printFrame n frame
    case cliCount cli of
      Just limit -> return (n + 1 < limit)
      Nothing    -> return True

  putStrLn "[demod-rx] RX complete"
