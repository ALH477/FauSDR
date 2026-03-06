-- Main.hs — DeMoD Faust-SDR Haskell TX entry point
-- DeMoD LLC
--
-- Usage:
--   demod-sdr-hs [--driver rtlsdr] [--freq 433.92e6] [--rate 2e6] [--gain 20]
--
-- Sends a stream of beacon frames with incrementing sequence numbers and
-- a 4-byte payload counter embedded in the payload field.

module Main where

import Control.Monad         (forM_, forever, when)
import Data.IORef            (newIORef, readIORef, modifyIORef')
import Data.Word             (Word8, Word16, Word32)
import System.Environment    (getArgs)
import System.Exit           (exitFailure)
import Text.Read             (readMaybe)

import DCF.Transport.Frame
import DCF.Modulator

-- ── CLI parsing ───────────────────────────────────────────────────────────────

data CLI = CLI
  { cliDriver :: String
  , cliFreq   :: Double
  , cliRate   :: Double
  , cliGain   :: Double
  } deriving (Show)

defaultCLI :: CLI
defaultCLI = CLI
  { cliDriver = "rtlsdr"
  , cliFreq   = 433.92e6
  , cliRate   = 2e6
  , cliGain   = 20.0
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
parseCLI (flag : _) _ = Left $ "Unknown flag: " <> flag

-- ── Frame source ──────────────────────────────────────────────────────────────

-- | Produce an infinite stream of beacon frames.
--   The 4-byte payload encodes a 32-bit counter (big-endian) for easy
--   packet counting on the RX side.
makeBeaconSource :: IO (IO (Maybe (DeModFrame, ())))
makeBeaconSource = do
  counterRef <- newIORef (0 :: Word32)
  return $ do
    c <- readIORef counterRef
    modifyIORef' counterRef (+1)
    let p0 = fromIntegral $ (c `div` 0x1000000) .&. 0xFF
        p1 = fromIntegral $ (c `div` 0x10000  ) .&. 0xFF
        p2 = fromIntegral $ (c `div` 0x100    ) .&. 0xFF
        p3 = fromIntegral $  c                   .&. 0xFF
    let frame = DeModFrame
          { frameVersion     = 1
          , frameType        = FBeacon
          , frameSeq         = 0     -- filled by runModulator
          , frameSrcId       = 0x0001
          , frameDstId       = broadcast
          , framePayload     = (p0, p1, p2, p3)
          , frameTimestampUs = 0     -- filled by runModulator
          }
    putStrLn $ "[demod-hs] beacon #" <> show c
    return (Just (frame, ()))

-- ── Main ─────────────────────────────────────────────────────────────────────

main :: IO ()
main = do
  args <- getArgs
  cli  <- case parseCLI args defaultCLI of
    Left err -> do
      putStrLn $ "Error: " <> err
      putStrLn "Usage: demod-sdr-hs [--driver NAME] [--freq HZ] [--rate HZ] [--gain DB]"
      exitFailure
    Right c  -> return c

  putStrLn $ "[demod-hs] DeMoD Faust-SDR Haskell TX"
  putStrLn $ "[demod-hs] driver=" <> cliDriver cli
  putStrLn $ "[demod-hs] freq="   <> show (cliFreq cli / 1e6) <> " MHz"
  putStrLn $ "[demod-hs] rate="   <> show (cliRate cli / 1e6) <> " MSPS"
  putStrLn $ "[demod-hs] gain="   <> show (cliGain cli)       <> " dB"

  let cfg = defaultModulatorConfig
        { modSdr = (modSdr defaultModulatorConfig)
            { sdrDriver       = cliDriver cli
            , sdrCenterFreqHz = cliFreq cli
            , sdrSampleRateHz = cliRate cli
            , sdrGainDb       = cliGain cli
            }
        , modDsp = (modDsp defaultModulatorConfig)
            { dspSampleRate = round (cliRate cli) }
        , modSymbol = (modSymbol defaultModulatorConfig)
            { symSampleRate = cliRate cli }
        }

  src <- makeBeaconSource
  putStrLn "[demod-hs] Starting TX loop — Ctrl+C to stop"
  runModulator cfg src
