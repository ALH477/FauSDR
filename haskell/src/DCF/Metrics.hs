-- DCF.Sim.Metrics — per-scenario metrics with pass/fail thresholds
-- DeMoD LLC

module DCF.Sim.Metrics
  ( Metrics
  , newMetrics
  , recordTx
  , recordRx
  , recordDrop
  , recordCrcFail
  , getReport
  , SimReport (..)
  , deliveryOk      -- ^ delivery rate >= threshold
  , latencyOk       -- ^ p99 latency <= threshold
  , printReport
  ) where

import Data.List   (sort)
import Data.IORef
import Text.Printf (printf)

data Metrics = Metrics
  { mTxCount   :: IORef Int
  , mRxCount   :: IORef Int
  , mDropCount :: IORef Int
  , mCrcFails  :: IORef Int
  , mLatencies :: IORef [Int]
  }

newMetrics :: IO Metrics
newMetrics = Metrics
  <$> newIORef 0
  <*> newIORef 0
  <*> newIORef 0
  <*> newIORef 0
  <*> newIORef []

recordTx :: Metrics -> IO ()
recordTx m = modifyIORef' (mTxCount m) (+1)

recordRx :: Metrics -> Int -> IO ()
recordRx m latUs = do
  modifyIORef' (mRxCount   m) (+1)
  modifyIORef' (mLatencies m) (latUs :)

recordDrop :: Metrics -> IO ()
recordDrop m = modifyIORef' (mDropCount m) (+1)

recordCrcFail :: Metrics -> IO ()
recordCrcFail m = modifyIORef' (mCrcFails m) (+1)

-- ── Report ────────────────────────────────────────────────────────────────────

data SimReport = SimReport
  { repTxCount      :: Int
  , repRxCount      :: Int
  , repDropCount    :: Int
  , repCrcFails     :: Int
  , repDeliveryPct  :: Double
  , repLatMinUs     :: Int
  , repLatMeanUs    :: Double
  , repLatP99Us     :: Int
  , repLatMaxUs     :: Int
  } deriving (Show)

getReport :: Metrics -> IO SimReport
getReport m = do
  tx   <- readIORef (mTxCount   m)
  rx   <- readIORef (mRxCount   m)
  drop <- readIORef (mDropCount m)
  crc  <- readIORef (mCrcFails  m)
  lats <- sort <$> readIORef (mLatencies m)

  let delivery = if tx == 0 then 0.0
                 else 100.0 * fromIntegral rx / fromIntegral tx
      latMin   = if null lats then 0 else head lats
      latMax   = if null lats then 0 else last lats
      latMean  = if null lats then 0.0
                 else fromIntegral (sum lats) / fromIntegral (length lats)
      latP99   = if null lats then 0
                 else lats !! max 0 (length lats * 99 `div` 100)

  return SimReport
    { repTxCount      = tx
    , repRxCount      = rx
    , repDropCount    = drop
    , repCrcFails     = crc
    , repDeliveryPct  = delivery
    , repLatMinUs     = latMin
    , repLatMeanUs    = latMean
    , repLatP99Us     = latP99
    , repLatMaxUs     = latMax
    }

-- ── Pass/fail predicates ──────────────────────────────────────────────────────

deliveryOk :: Double -> SimReport -> Bool
deliveryOk threshold r = repDeliveryPct r >= threshold

latencyOk :: Int -> SimReport -> Bool
latencyOk thresholdUs r = repLatP99Us r <= thresholdUs

-- ── Pretty print ─────────────────────────────────────────────────────────────

printReport :: String -> SimReport -> IO ()
printReport label r = do
  let w = 52
      bar = replicate w '─'
  putStrLn $ "  ┌" <> bar <> "┐"
  putStrLn $ "  │  " <> padR (w - 2) label <> "│"
  putStrLn $ "  ├" <> bar <> "┤"
  row "Frames TX"       $ show (repTxCount r)
  row "Frames RX"       $ show (repRxCount r)
  row "Dropped"         $ show (repDropCount r)
  row "CRC failures"    $ show (repCrcFails r)
  row "Delivery"        $ printf "%.2f%%" (repDeliveryPct r)
  row "Latency min"     $ show (repLatMinUs r) <> " µs"
  row "Latency mean"    $ printf "%.1f µs" (repLatMeanUs r)
  row "Latency p99"     $ show (repLatP99Us r) <> " µs"
  row "Latency max"     $ show (repLatMaxUs r) <> " µs"
  putStrLn $ "  └" <> bar <> "┘"
  where
    row k v =
      let kw = 16
          vw = w - kw - 2
          line = "  │  " <> padR kw k <> padL vw v <> "  │"
      in putStrLn line
    padR n s = take n (s <> repeat ' ')
    padL n s = replicate (n - length s) ' ' <> s
