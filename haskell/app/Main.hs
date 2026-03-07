-- Main.hs — DeMoD Faust-SDR Haskell TX entry point
-- DeMoD LLC
--
-- Usage:
--   demod-sdr-hs [--driver rtlsdr] [--freq 433.92e6] [--rate 2e6] [--gain 20]
--
-- Sends a stream of beacon frames with incrementing sequence numbers and
-- a 4-byte payload counter embedded in the payload field.
--
-- BUG FIX: previous version was missing `Data.Bits` import required for
-- (.&.) in the payload byte extraction.  runModulator signature changed:
-- frame source now yields Maybe DeModFrame (not Maybe (DeModFrame, ())).

module Main where

import Data.Bits             ((.&.), shiftR)
import Data.IORef            (newIORef, readIORef, modifyIORef')
import Data.Word             (Word32)
import System.Environment    (getArgs)
import System.Exit           (exitFailure)
import Text.Read             (readMaybe)

import DCF.Transport.Frame
import DCF.Modulator
import DCF.Faust.DSP       (DspConfig (..))
import DCF.SDR.Device      (SdrConfig (..))
import DCF.Transport.Symbol (SymbolConfig (..))

-- ── CLI parsing ───────────────────────────────────────────────────────────────

data CLI = CLI
  { cliDriver :: String
  , cliFreq   :: Double
  , cliRate   :: Double
  , cliGain   :: Double
  , cliCount  :: Maybe Int   -- ^ stop after N frames (Nothing = infinite)
  } deriving (Show)

defaultCLI :: CLI
defaultCLI = CLI
  { cliDriver = "rtlsdr"
  , cliFreq   = 433.92e6
  , cliRate   = 2e6
  , cliGain   = 20.0
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
parseCLI ("--help" : _) _ = Left ""   -- trigger usage print
parseCLI (flag : _) _     = Left $ "Unknown flag: " <> flag

usage :: String
usage = unlines
  [ "Usage: demod-sdr-hs [OPTIONS]"
  , ""
  , "  --driver NAME   SoapySDR driver  (default: rtlsdr)"
  , "  --freq   HZ     Centre frequency (default: 433.92e6)"
  , "  --rate   HZ     Sample rate      (default: 2e6)"
  , "  --gain   DB     TX gain          (default: 20)"
  , "  --count  N      Stop after N frames (default: infinite)"
  , "  --help          Show this message"
  ]

-- ── Frame source ──────────────────────────────────────────────────────────────

-- | Produce an infinite (or bounded) stream of beacon frames.
--   The 4-byte payload encodes a big-endian 32-bit counter for easy
--   packet counting on the RX side.
makeBeaconSource :: Maybe Int -> IO (IO (Maybe DeModFrame))
makeBeaconSource limit = do
  counterRef <- newIORef (0 :: Word32)
  return $ do
    c <- readIORef counterRef
    -- Stop when limit reached
    case limit of
      Just n | fromIntegral c >= n -> return Nothing
      _                            -> do
        modifyIORef' counterRef (+ 1)
        let p0 = fromIntegral $ (c `shiftR` 24) .&. 0xFF
            p1 = fromIntegral $ (c `shiftR` 16) .&. 0xFF
            p2 = fromIntegral $ (c `shiftR`  8) .&. 0xFF
            p3 = fromIntegral $  c              .&. 0xFF
            frame = DeModFrame
              { frameVersion     = 1
              , frameType        = FBeacon
              , frameSeq         = 0     -- filled by runModulator
              , frameSrcId       = 0x0001
              , frameDstId       = broadcast
              , framePayload     = (p0, p1, p2, p3)
              , frameTimestampUs = 0     -- filled by runModulator
              }
        putStrLn $ "[demod-hs] beacon #" <> show c
        return (Just frame)

-- ── Main ─────────────────────────────────────────────────────────────────────

main :: IO ()
main = do
  args <- getArgs
  cli  <- case parseCLI args defaultCLI of
    Left "" -> do
      putStr usage
      exitFailure
    Left err -> do
      putStrLn $ "Error: " <> err
      putStr usage
      exitFailure
    Right c  -> return c

  putStrLn "[demod-hs] DeMoD Faust-SDR Haskell TX"
  putStrLn $ "[demod-hs] driver=" <> cliDriver cli
  putStrLn $ "[demod-hs] freq="   <> show (cliFreq cli / 1e6) <> " MHz"
  putStrLn $ "[demod-hs] rate="   <> show (cliRate cli / 1e6) <> " MSPS"
  putStrLn $ "[demod-hs] gain="   <> show (cliGain cli)       <> " dB"
  putStrLn $ "[demod-hs] count="  <> maybe "infinite" show (cliCount cli)

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

  src <- makeBeaconSource (cliCount cli)
  putStrLn "[demod-hs] Starting TX loop — Ctrl+C to stop"
  runModulator cfg src
  putStrLn "[demod-hs] TX complete"
