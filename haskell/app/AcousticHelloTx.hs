-- app/AcousticHelloTx.hs — DeMoD Acoustic FSK "Hello World" TX
-- DeMoD LLC | GPL-3.0
--
-- Sends "HELLO" as JackFrame packets over the acoustic FSK modem.
-- Each frame carries 4 bytes of payload:
--
--   Frame 0 · DATA   · seq=0 · 48 45 4C 4C  (HELL)
--   Frame 1 · DATA   · seq=1 · 4F 00 00 00  (O...)
--   Frame 2 · BEACON · seq=2               (EOM)
--
-- Usage:
--   cabal run acoustic-hello-tx
--   cabal run acoustic-hello-tx -- --gain 0.5 --count 3
--
-- Options:
--   --gain  FLOAT   Speaker output gain 0.0–1.0  (default: 0.7)
--   --rate  INT     Sample rate Hz               (default: 48000)
--   --baud  FLOAT   Symbol rate baud             (default: 1200)
--   --count INT     Repeat message N times       (default: 1)
--
-- Expected output:
--   [hello-tx] DeMoD Acoustic FSK — Hello World TX
--   [hello-tx] 2000/3000 Hz · 1200 baud · 48000 Hz SR · sps=40
--   [hello-tx] Transmitting: "HELLO"  (3 frames · ~202ms)
--   [hello-tx]
--   [hello-tx] frame 0/2 · DATA   · seq=0 · 48 45 4C 4C  (HELL)
--   [hello-tx] frame 1/2 · DATA   · seq=1 · 4F 00 00 00  (O...)
--   [hello-tx] frame 2/2 · BEACON · seq=2               (EOM)
--   [hello-tx]
--   [hello-tx] Done. 3 frames · 202ms total airtime
--   [hello-tx] It sounded exactly like a dial-up modem.

module Main where

import Control.Concurrent         (forkIO, threadDelay)
import Control.Exception          (catch, SomeException)
import Control.Monad              (forM_, unless)
import Data.Char                  (ord)
import Data.IORef
import Data.List                  (intercalate)
import Data.Word                  (Word8)
import Data.Vector.Storable       (Vector)
import qualified Data.Vector.Storable as V
import Foreign.C.Error            (Errno(..))
import Foreign.C.Types            (CFloat(..))
import Foreign.Ptr                (castPtr, Ptr)
import Foreign.Storable           (pokeElemOff)
import System.Environment         (getArgs)
import System.Exit                (exitFailure)
import System.IO                  (hFlush, stdout)
import Text.Printf                (printf)
import Text.Read                  (readMaybe)

import qualified Sound.JACK                              as JACK
import qualified Sound.JACK.Audio                        as JAudio
import qualified Control.Monad.Exception.Synchronous    as Sync
import qualified Control.Monad.Trans.Class              as Trans

import DCF.Faust.DSP
import DCF.Modem.AcousticFSK
import DCF.Transport.JackFrame

-- ── CLI ───────────────────────────────────────────────────────────────────────

data CLI = CLI
  { cliGain  :: Double
  , cliRate  :: Int
  , cliBaud  :: Double
  } deriving (Show)

defaultCLI :: CLI
defaultCLI = CLI { cliGain = 0.7, cliRate = 48000, cliBaud = 1200.0 }

parseCLI :: [String] -> CLI -> Either String CLI
parseCLI [] c = Right c
parseCLI ("--gain"  : v : rest) c =
  maybe (Left $ "Invalid --gain: " <> v)  (\d -> parseCLI rest c { cliGain  = d }) (readMaybe v)
parseCLI ("--rate"  : v : rest) c =
  maybe (Left $ "Invalid --rate: " <> v)  (\n -> parseCLI rest c { cliRate  = n }) (readMaybe v)
parseCLI ("--baud"  : v : rest) c =
  maybe (Left $ "Invalid --baud: " <> v)  (\d -> parseCLI rest c { cliBaud  = d }) (readMaybe v)
parseCLI ("--help" : _) _ = Left ""
parseCLI (f : _) _        = Left $ "Unknown flag: " <> f

usage :: String
usage = unlines
  [ "Usage: acoustic-hello-tx [OPTIONS]"
  , "  --gain  FLOAT   Speaker gain 0.0-1.0  (default: 0.7)"
  , "  --rate  INT     Sample rate Hz        (default: 48000)"
  , "  --baud  FLOAT   Symbol rate baud      (default: 1200)"
  ]

-- ── Message → JackFrames ──────────────────────────────────────────────────────

-- | Pack a String into DATA JackFrames (4 bytes/frame) + a BEACON EOM frame.
messageToFrames :: String -> [JackFrame]
messageToFrames msg =
  let bytes   = map (fromIntegral . ord) msg :: [Word8]
      nChunks = (length bytes + 3) `div` 4
      chunks  = take nChunks $ chunksOf 4 (bytes ++ repeat 0x00)
      dataFs  = zipWith mkData [0..] chunks
      eomF    = JackFrame (fromIntegral nChunks `mod` 16)
                          JFBeacon (0x45, 0x4F, 0x4D, 0x00)  -- "EOM\0"
  in  dataFs ++ [eomF]

mkData :: Int -> [Word8] -> JackFrame
mkData i bs =
  let padded = bs ++ repeat 0
      b0 = padded !! 0
      b1 = padded !! 1
      b2 = padded !! 2
      b3 = padded !! 3
  in  JackFrame (fromIntegral i `mod` 16) JFData (b0,b1,b2,b3)

chunksOf :: Int -> [a] -> [[a]]
chunksOf _ [] = []
chunksOf n xs = let (h,t) = splitAt n xs in h : chunksOf n t

-- ── Display ───────────────────────────────────────────────────────────────────

hexPay :: (Word8,Word8,Word8,Word8) -> String
hexPay (a,b,c,d) = intercalate " " $ map (printf "%02X") [a,b,c,d]

asciiPay :: (Word8,Word8,Word8,Word8) -> String
asciiPay (a,b,c,d) = map (\w -> if w >= 0x20 && w <= 0x7E
                                 then toEnum (fromIntegral w) else '.') [a,b,c,d]

printFrame :: Int -> Int -> JackFrame -> IO ()
printFrame idx total f = case jfType f of
  JFData   -> printf "[hello-tx] frame %d/%d · DATA   · seq=%d · %s  (%s)\n"
                idx total (jfSeq f) (hexPay (jfPayload f)) (asciiPay (jfPayload f))
  JFBeacon -> printf "[hello-tx] frame %d/%d · BEACON · seq=%d               (EOM)\n"
                idx total (jfSeq f)
  t        -> printf "[hello-tx] frame %d/%d · %s · seq=%d\n"
                idx total (show t) (jfSeq f)

-- ── Symbol vector → audio via Faust ──────────────────────────────────────────

runFaustTx :: DspHandle -> Int -> Vector Float -> IO (Vector Float)
runFaustTx dsp blk symVec = do
  let n       = V.length symVec
      nBlocks = (n + blk - 1) `div` blk
      padded  = symVec V.++ V.replicate (nBlocks * blk - n) 0.0
  audioBlocks <- mapM (processBlock padded) [0 .. nBlocks - 1]
  return $ V.take n (V.concat audioBlocks)
  where
    processBlock vec i = do
      let chunk = V.slice (i * blk) blk vec
      outChannels <- compute dsp [chunk] blk
      return $ case outChannels of
        (audio : _) -> audio
        []          -> V.replicate blk 0.0

-- ── TX Playback Buffer ────────────────────────────────────────────────────────
--
-- A simple IORef-backed FIFO of CFloat samples.
-- The JACK process callback drains this; writeAudio enqueues into it.
-- Audio is generated faster than real-time; drainTxBuf paces the TX loop.

newtype TxBuf = TxBuf { tbSamples :: IORef [CFloat] }

newTxBuf :: IO TxBuf
newTxBuf = TxBuf <$> newIORef []

-- | Enqueue a block of Float samples for JACK playback.
appendTxBuf :: TxBuf -> Vector Float -> IO ()
appendTxBuf tb vec =
  modifyIORef' (tbSamples tb) (++ map CFloat (V.toList vec))

-- | Block the calling thread until the JACK callback has consumed all
--   queued samples (i.e. the audio has actually been played out).
drainTxBuf :: TxBuf -> IO ()
drainTxBuf tb = go
  where
    go = do
      xs <- readIORef (tbSamples tb)
      unless (null xs) $ threadDelay 10_000 >> go

-- ── JACK TX Process Callback ──────────────────────────────────────────────────
--
-- Runs in the JACK real-time thread. Drains TxBuf into the output port buffer,
-- padding with silence on underrun.

txJackCallback
    :: TxBuf
    -> JACK.Port JAudio.Sample JACK.Output
    -> JACK.NFrames
    -> Sync.ExceptionalT Errno IO ()
txJackCallback tb outPort nframes = Trans.lift $ do
  let JACK.NFrames w = nframes
      n = fromIntegral w :: Int
  rawPtr <- JAudio.getBufferPtr outPort nframes
  let outPtr = castPtr rawPtr :: Ptr CFloat
  xs <- readIORef (tbSamples tb)
  -- Write n samples: drain from queue, pad with 0 on underrun.
  forM_ (zip [0 .. n-1] (xs ++ repeat (CFloat 0.0))) $ \(i, s) ->
    pokeElemOff outPtr i s
  writeIORef (tbSamples tb) (drop n xs)

-- ── JACK TX Session ───────────────────────────────────────────────────────────
--
-- Opens a JACK client with a single output port, installs txJackCallback,
-- and blocks until the process exits.  Runs in a background forkIO thread.

jackTxSession :: TxBuf -> IO ()
jackTxSession tb =
  JACK.handleExceptions $
    JACK.withClientDefault "acoustic-hello-tx" $ \client ->
      JACK.withPort client "output" $ \outPort ->
        JACK.withProcess client (txJackCallback tb outPort) $
          JACK.withActivation client $
            Trans.lift $ do
              putStrLn "[hello-tx] JACK output port active."
              putStrLn "[hello-tx] Connect with: jack_connect acoustic-hello-tx:output system:playback_1"
              -- Block indefinitely; killed when the main thread exits.
              let idle = threadDelay 1_000_000 >> idle
              idle

-- ── Audio output ──────────────────────────────────────────────────────────────
--
-- Enqueue synthesised audio into TxBuf for the JACK callback to play.
-- The top-level binding is a placeholder; main rebinds it as a closure
-- capturing the live TxBuf.

writeAudio :: TxBuf -> Int -> Vector Float -> IO ()
writeAudio tb _sr vec = appendTxBuf tb vec

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
              , acOutputGain = cliGain cli
              }
      sps    = acousticSps cfg
      dspCfg = defaultDspConfig
                 { dspSampleRate    = cliRate cli
                 , dspExpectInputs  = 1   -- symbol stream in
                 , dspExpectOutputs = 1   -- mono audio out
                 }

  -- Banner
  putStrLn "[hello-tx] DeMoD Acoustic FSK TX"
  printf   "[hello-tx] %.0f/%.0f Hz · %.0f baud · %d Hz SR · sps=%d\n"
    (acFMark cfg) (acFSpace cfg) (acSymbolRate cfg) (cliRate cli) sps
  putStrLn "[hello-tx] Type a message and press Enter to transmit."
  putStrLn "[hello-tx] Empty line or Ctrl+C to quit."
  putStrLn "[hello-tx]"

  -- Start JACK output session in a background thread.
  tb <- newTxBuf
  _  <- forkIO $
    jackTxSession tb
      `catch` \(e :: SomeException) ->
        putStrLn ("[hello-tx] JACK error: " ++ show e)
  -- Give JACK time to connect before the first frame is sent.
  threadDelay 500_000

  -- Open DSP once, prompt in a loop.
  withDsp dspCfg $ \dsp -> do
    setAcousticTxParams dsp cfg
    let loop = do
          putStr "send> "
          hFlush stdout
          line <- getLine
          case line of
            "" -> putStrLn "[hello-tx] Bye."
            msg -> do
              let frames    = messageToFrames msg
                  nFrames   = length frames
                  airtimeMs = 64.0 / cliBaud cli * 1000
                  frameMs   = airtimeMs
                            + fromIntegral (acPreambleLen cfg) / cliBaud cli * 1000
                            + fromIntegral (acSilenceLen  cfg) / fromIntegral (cliRate cli) * 1000
                  totalMs   = fromIntegral nFrames * frameMs
              printf "[hello-tx] \"%s\" -> %d frames, ~%.0fms\n" msg nFrames totalMs
              forM_ (zip [0..] frames) $ \(idx, frame) -> do
                printFrame idx (nFrames - 1) frame
                let symVec = encodeAcousticFrame cfg frame
                audioVec  <- runFaustTx dsp (acBlockSize cfg) symVec
                writeAudio tb (cliRate cli) audioVec
              -- Wait for the JACK callback to finish playing all queued audio
              -- before accepting the next message.
              drainTxBuf tb
              putStrLn "[hello-tx] Sent."
              putStrLn "[hello-tx]"
              loop
    loop
