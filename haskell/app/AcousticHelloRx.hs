-- app/AcousticHelloRx.hs — DeMoD Acoustic FSK "Hello World" RX
-- DeMoD LLC | GPL-3.0
--
-- Listens for JackFrame packets over the acoustic FSK modem and reassembles
-- them into a message string. Prints each frame as it arrives, then prints
-- the full reconstructed message when a BEACON EOM frame is received.
--
-- Usage:
--   cabal run acoustic-hello-rx
--   cabal run acoustic-hello-rx -- --bpf-q 5.0 --timeout 10
--
-- Options:
--   --rate     INT     Sample rate Hz            (default: 48000)
--   --baud     FLOAT   Symbol rate baud          (default: 1200)
--   --bpf-q   FLOAT   BPF Q factor              (default: 3.5)
--   --gain    FLOAT   Soft symbol output gain   (default: 4.0)
--   --timeout  INT     Stop after N seconds      (default: infinite)
--
-- Run RX before TX. The first few frames may be dropped while the mic
-- AGC settles (~160ms at default acAcqFrames=3). This is normal.
--
-- Expected output:
--   [hello-rx] DeMoD Acoustic FSK — Hello World RX
--   [hello-rx] 2000/3000 Hz · 1200 baud · 48000 Hz SR
--   [hello-rx] Listening... (Ctrl+C to stop)
--   [hello-rx]
--   [hello-rx] Waiting for mic AGC to settle...
--   [hello-rx] Ready.
--   [hello-rx]
--   [hello-rx] frame · DATA   · seq=0 · 48 45 4C 4C  (HELL)
--   [hello-rx] frame · DATA   · seq=1 · 4F 00 00 00  (O...)
--   [hello-rx] frame · BEACON · seq=2               (EOM)
--   [hello-rx] ─────────────────────────────
--   [hello-rx] Message: "HELLO"
--   [hello-rx] ─────────────────────────────
--   [hello-rx] Frames: 2 data · 1 beacon · 0 dropped
--   [hello-rx] Listening...

module Main where

import Control.Monad         (when)
import Data.Char             (chr, isPrint)
import Data.IORef
import Data.List             (intercalate)
import Data.Word             (Word8)
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable as V
import System.Environment    (getArgs)
import System.Exit           (exitFailure)
import Text.Printf           (printf)
import Text.Read             (readMaybe)

import DCF.Faust.DSP
import DCF.Modem.AcousticFSK
import DCF.Transport.JackFrame

-- ── CLI ───────────────────────────────────────────────────────────────────────

data CLI = CLI
  { cliRate    :: Int
  , cliBaud    :: Double
  , cliBpfQ    :: Double
  , cliGain    :: Double
  , cliTimeout :: Maybe Int
  } deriving (Show)

defaultCLI :: CLI
defaultCLI = CLI
  { cliRate    = 48000
  , cliBaud    = 1200.0
  , cliBpfQ    = 3.5
  , cliGain    = 4.0
  , cliTimeout = Nothing
  }

parseCLI :: [String] -> CLI -> Either String CLI
parseCLI [] c = Right c
parseCLI ("--rate"    : v : rest) c =
  maybe (Left $ "Invalid --rate: "    <> v) (\n -> parseCLI rest c { cliRate    = n }) (readMaybe v)
parseCLI ("--baud"    : v : rest) c =
  maybe (Left $ "Invalid --baud: "    <> v) (\d -> parseCLI rest c { cliBaud    = d }) (readMaybe v)
parseCLI ("--bpf-q"   : v : rest) c =
  maybe (Left $ "Invalid --bpf-q: "   <> v) (\d -> parseCLI rest c { cliBpfQ    = d }) (readMaybe v)
parseCLI ("--gain"    : v : rest) c =
  maybe (Left $ "Invalid --gain: "    <> v) (\d -> parseCLI rest c { cliGain    = d }) (readMaybe v)
parseCLI ("--timeout" : v : rest) c =
  maybe (Left $ "Invalid --timeout: " <> v) (\n -> parseCLI rest c { cliTimeout = Just n }) (readMaybe v)
parseCLI ("--help" : _) _ = Left ""
parseCLI (f : _) _        = Left $ "Unknown flag: " <> f

usage :: String
usage = unlines
  [ "Usage: acoustic-hello-rx [OPTIONS]"
  , "  --rate    INT    Sample rate Hz          (default: 48000)"
  , "  --baud    FLOAT  Symbol rate baud        (default: 1200)"
  , "  --bpf-q  FLOAT  BPF Q factor            (default: 3.5)"
  , "  --gain   FLOAT  Soft symbol scale        (default: 4.0)"
  , "  --timeout INT   Stop after N seconds     (default: infinite)"
  ]

-- ── Payload extraction ────────────────────────────────────────────────────────

payloadBytes :: (Word8,Word8,Word8,Word8) -> [Word8]
payloadBytes (b0,b1,b2,b3) = [b0,b1,b2,b3]

-- Strip trailing null bytes from a byte list and decode as ASCII.
-- Non-printable bytes are replaced with '.'.
bytesToString :: [Word8] -> String
bytesToString = reverse . dropWhile (== '\0') . reverse . map toChar
  where toChar b = let c = chr (fromIntegral b) in if isPrint c then c else '.'

hexPay :: (Word8,Word8,Word8,Word8) -> String
hexPay (a,b,c,d) = intercalate " " $ map (printf "%02X") [a,b,c,d]

asciiPay :: (Word8,Word8,Word8,Word8) -> String
asciiPay (a,b,c,d) = map (\w -> if w >= 0x20 && w <= 0x7E
                                 then toEnum (fromIntegral w) else '.') [a,b,c,d]

printRxFrame :: JackFrame -> IO ()
printRxFrame f = case jfType f of
  JFData   -> printf "[hello-rx] frame · DATA   · seq=%d · %s  (%s)\n"
                (jfSeq f) (hexPay (jfPayload f)) (asciiPay (jfPayload f))
  JFBeacon -> printf "[hello-rx] frame · BEACON · seq=%d               (EOM)\n"
                (jfSeq f)
  t        -> printf "[hello-rx] frame · %s · seq=%d\n" (show t) (jfSeq f)

-- ── Audio input ──────────────────────────────────────────────────────────────
--
-- Stub: replace with JACK port read or PipeWire stream read.
-- In the real integration:
--   JACK.Process.readPortBuffer inPort nframes >>= return . vectorFromBuffer
--
-- For testing, feed raw f32 from arecord:
--   arecord -f FLOAT_LE -r 48000 -c 1 /dev/stdin | \
--     cabal run acoustic-hello-rx
readAudioBlock :: Int -> Int -> IO (Vector Float)
readAudioBlock _sr blk = return (V.replicate blk 0.0)   -- stub

-- ── Soft samples → bits via Faust ────────────────────────────────────────────
--
-- acoustic_fsk_demod: 1 input (mono mic audio), 1 output (soft bit decisions).
-- DCF.Faust.DSP.compute :: DspHandle -> [Vector Float] -> Int -> IO [Vector Float]

runFaustRx :: DspHandle -> Int -> Vector Float -> IO (Vector Float)
runFaustRx dsp blk micVec = do
  outChannels <- compute dsp [micVec] blk
  return $ case outChannels of
    (soft : _) -> soft
    []         -> V.replicate blk 0.0

-- ── Frame reassembly ─────────────────────────────────────────────────────────

-- | Accumulate DATA frame payloads until a BEACON EOM frame arrives.
--   Returns the reassembled message string.
type MsgBuf = IORef [Word8]

newMsgBuf :: IO MsgBuf
newMsgBuf = newIORef []

pushPayload :: MsgBuf -> (Word8,Word8,Word8,Word8) -> IO ()
pushPayload buf payload = modifyIORef' buf (<> payloadBytes payload)

flushMessage :: MsgBuf -> IO String
flushMessage buf = do
  bytes <- readIORef buf
  writeIORef buf []
  return $ bytesToString bytes

-- ── Stats ────────────────────────────────────────────────────────────────────

data Stats = Stats
  { statData    :: Int
  , statBeacon  :: Int
  , statDropped :: Int
  }

newStats :: IO (IORef Stats)
newStats = newIORef (Stats 0 0 0)

bumpData, bumpBeacon, bumpDropped :: IORef Stats -> IO ()
bumpData    s = modifyIORef' s $ \st -> st { statData    = statData    st + 1 }
bumpBeacon  s = modifyIORef' s $ \st -> st { statBeacon  = statBeacon  st + 1 }
bumpDropped s = modifyIORef' s $ \st -> st { statDropped = statDropped st + 1 }

printStats :: IORef Stats -> IO ()
printStats s = do
  st <- readIORef s
  printf "[hello-rx] Frames: %d data · %d beacon · %d dropped\n"
    (statData st) (statBeacon st) (statDropped st)

-- ── Main ─────────────────────────────────────────────────────────────────────

main :: IO ()
main = do
  args <- getArgs
  cli  <- case parseCLI args defaultCLI of
    Left ""  -> putStr usage >> exitFailure
    Left err -> putStrLn ("Error: " <> err) >> putStr usage >> exitFailure
    Right c  -> return c

  let cfg = defaultAcousticConfig
              { acSampleRate = fromIntegral (cliRate cli)
              , acSymbolRate = cliBaud cli
              , acBpfQ       = cliBpfQ cli
              , acDemodGain  = cliGain cli
              }
      sps    = acousticSps cfg
      dspCfg = defaultDspConfig
                 { dspSampleRate    = cliRate cli
                 , dspExpectInputs  = 1   -- mono mic audio
                 , dspExpectOutputs = 1   -- soft bit decisions
                 }

  -- Banner
  putStrLn "[hello-rx] DeMoD Acoustic FSK — Hello World RX"
  printf   "[hello-rx] %.0f/%.0f Hz · %.0f baud · %d Hz SR · sps=%d\n"
    (acFMark cfg) (acFSpace cfg) (acSymbolRate cfg) (cliRate cli) sps
  putStrLn "[hello-rx] Listening... (Ctrl+C to stop)"
  putStrLn "[hello-rx]"

  msgBuf   <- newMsgBuf
  statsRef <- newStats
  bitBuf   <- newIORef ([] :: [Bool])

  -- Discard initial blocks while mic AGC settles
  putStrLn "[hello-rx] Waiting for mic AGC to settle..."
  let acqSamples = acAcqFrames cfg * acousticFrameSamples cfg
      acqBlocks  = (acqSamples + acBlockSize cfg - 1) `div` acBlockSize cfg
  mapM_ (\_ -> readAudioBlock (cliRate cli) (acBlockSize cfg)) [1..acqBlocks]
  putStrLn "[hello-rx] Ready."
  putStrLn "[hello-rx]"

  -- RX loop
  withDsp dspCfg $ \dsp -> do
    setAcousticRxParams dsp cfg
    let loop = do
          -- 1. Read mic block from JACK/PipeWire
          micBlock <- readAudioBlock (cliRate cli) (acBlockSize cfg)

          -- 2. Feed through Faust acoustic_fsk_demod → soft bit decisions
          softBlock <- runFaustRx dsp (acBlockSize cfg) micBlock

          -- 3. Slice at symbol centres (every sps samples, offset sps/2)
          let newBits = sliceAcousticSymbols sps softBlock

          -- 4. Append to bit buffer, try to find a complete frame
          modifyIORef' bitBuf (<> newBits)
          drainFrames msgBuf statsRef bitBuf
          loop

    loop

  where
    -- Try to extract all complete JackFrames from the bit buffer.
    -- findJackFrame does byte-aligned sliding sync search with CRC gating.
    drainFrames msgBuf statsRef bitBuf = do
      bits <- readIORef bitBuf
      case findJackFrame bits of
        Nothing             -> return ()
        Just (frame, rest)  -> do
          writeIORef bitBuf rest
          handleFrame msgBuf statsRef frame
          drainFrames msgBuf statsRef bitBuf

    handleFrame msgBuf statsRef frame = do
      printRxFrame frame
      case jfType frame of
        JFData -> do
          bumpData statsRef
          pushPayload msgBuf (jfPayload frame)

        JFBeacon -> do
          bumpBeacon statsRef
          -- EOM beacon — flush accumulated payload to message
          msg <- flushMessage msgBuf
          when (not (null msg)) $ do
            putStrLn "[hello-rx] ─────────────────────────────"
            printf   "[hello-rx] Message: \"%s\"\n" msg
            putStrLn "[hello-rx] ─────────────────────────────"
            printStats statsRef
            putStrLn "[hello-rx] Listening..."

        _ -> bumpDropped statsRef
