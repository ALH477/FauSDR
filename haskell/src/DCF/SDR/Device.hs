-- DCF.SDR.Device — safe SoapySDR TX/RX device handles
-- DeMoD LLC

module DCF.SDR.Device
  ( SdrConfig (..)
  , defaultSdrConfig
  , SdrDevice
  , withSdrDevice
  , writeSamples
    -- RX
  , SdrRxConfig (..)
  , defaultSdrRxConfig
  , SdrRxDevice
  , withSdrRxDevice
  , readSamples
  ) where

import Control.Exception     (bracket, throwIO, ErrorCall (..))
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable         as V
import qualified Data.Vector.Storable.Mutable as VM
import Foreign.C.String      (withCString)
import Foreign.C.Types       (CDouble (..), CFloat (..), CInt (..), CLong (..))
import Foreign.Ptr           (nullPtr, castPtr)

import DCF.SDR.SoapyFFI

-- ── TX Config ─────────────────────────────────────────────────────────────────

data SdrConfig = SdrConfig
  { sdrDriver        :: String
  , sdrCenterFreqHz  :: Double
  , sdrSampleRateHz  :: Double
  , sdrGainDb        :: Double
  , sdrTimeoutUs     :: Int
  } deriving (Show, Eq)

defaultSdrConfig :: SdrConfig
defaultSdrConfig = SdrConfig
  { sdrDriver       = "rtlsdr"
  , sdrCenterFreqHz = 433.92e6
  , sdrSampleRateHz = 2e6
  , sdrGainDb       = 20.0
  , sdrTimeoutUs    = 1_000_000
  }

-- ── TX Handle ─────────────────────────────────────────────────────────────────

data SdrDevice = SdrDevice
  { _dev    :: SoapyDev
  , _stream :: SoapyStream
  }

-- | Open the SDR device, configure TX, open and activate the CF32 TX stream.
--   Cleans up on exception or normal exit.
withSdrDevice :: SdrConfig -> (SdrDevice -> IO a) -> IO a
withSdrDevice cfg action =
  withCString (sdrDriver cfg) $ \driverC ->
  bracket (soapyOpen driverC) cleanup $ \dev -> do
    if dev == nullPtr
      then throwIO (ErrorCall $ "SoapySDR: could not open driver '" <> sdrDriver cfg <> "'")
      else do
        soapyConfigureTx dev
          (CDouble $ sdrCenterFreqHz cfg)
          (CDouble $ sdrSampleRateHz cfg)
          (CDouble $ sdrGainDb       cfg)
        stream <- soapyOpenTxStream dev
        if stream == nullPtr
          then throwIO (ErrorCall "SoapySDR: could not open TX stream")
          else action (SdrDevice dev stream)
  where
    cleanup dev
      | dev == nullPtr = pure ()
      | otherwise      = soapyClose dev

-- | Write interleaved CF32 samples [I0,Q0,I1,Q1,...].
--   Length must be even (2 * num_samples).
--   @timeoutUs@ must come from the caller's active SdrConfig.
--   Returns IQ pairs written (negative = SoapySDR error code).
writeSamples :: SdrDevice -> Vector Float -> Int -> IO Int
writeSamples (SdrDevice dev stream) iqVec timeoutUs = do
  let numSamples = V.length iqVec `div` 2
  V.unsafeWith iqVec $ \ptr ->
    fromIntegral <$>
      soapyWriteCF32 dev stream ptr
                     (fromIntegral numSamples)
                     (fromIntegral timeoutUs)

-- ── RX Config ─────────────────────────────────────────────────────────────────

data SdrRxConfig = SdrRxConfig
  { rxDriver        :: String
  , rxCenterFreqHz  :: Double
  , rxSampleRateHz  :: Double
  , rxGainDb        :: Double   -- ^ 30–50 dB typical for RTL-SDR
  , rxTimeoutUs     :: Int
  , rxBlockSize     :: Int      -- ^ IQ pairs per readSamples call
  } deriving (Show, Eq)

defaultSdrRxConfig :: SdrRxConfig
defaultSdrRxConfig = SdrRxConfig
  { rxDriver       = "rtlsdr"
  , rxCenterFreqHz = 433.92e6
  , rxSampleRateHz = 2e6
  , rxGainDb       = 40.0
  , rxTimeoutUs    = 1_000_000
  , rxBlockSize    = 4096
  }

-- ── RX Handle ─────────────────────────────────────────────────────────────────

data SdrRxDevice = SdrRxDevice
  { _rxDev    :: SoapyDev
  , _rxStream :: SoapyStream
  , _rxBlock  :: Int
  }

-- | Open a receive stream.
--   On full-duplex hardware (HackRF, USRP) TX and RX may be opened
--   simultaneously via separate withSdrDevice / withSdrRxDevice calls on the
--   same physical device — SoapySDR multiplexes them.  On RTL-SDR (RX-only)
--   only withSdrRxDevice is meaningful.
withSdrRxDevice :: SdrRxConfig -> (SdrRxDevice -> IO a) -> IO a
withSdrRxDevice cfg action =
  withCString (rxDriver cfg) $ \driverC ->
  bracket (soapyOpen driverC) cleanup $ \dev -> do
    if dev == nullPtr
      then throwIO (ErrorCall $ "SoapySDR: could not open RX driver '" <> rxDriver cfg <> "'")
      else do
        soapyConfigureRx dev
          (CDouble $ rxCenterFreqHz cfg)
          (CDouble $ rxSampleRateHz cfg)
          (CDouble $ rxGainDb       cfg)
        stream <- soapyOpenRxStream dev
        if stream == nullPtr
          then throwIO (ErrorCall "SoapySDR: could not open RX stream")
          else action (SdrRxDevice dev stream (rxBlockSize cfg))
  where
    cleanup dev
      | dev == nullPtr = pure ()
      | otherwise      = soapyClose dev

-- | Read one block of interleaved CF32 samples from the RX stream.
--
--   Allocates a pinned mutable buffer, calls soapy_read_cf32, then freezes.
--   Returns:
--     Right vec  — vec has length 2 * nRead (interleaved IQ pairs)
--     Left  0    — timeout; non-fatal, caller should retry
--     Left  n    — SoapySDR error (n < 0)
readSamples :: SdrRxDevice -> Int -> IO (Either Int (Vector Float))
readSamples (SdrRxDevice dev stream blockSize) timeoutUs = do
  let bufLen = blockSize * 2
  mvec <- VM.new bufLen
  n <- VM.unsafeWith mvec $ \ptr ->
    soapyReadCF32 dev stream (castPtr ptr)
                  (fromIntegral blockSize)
                  (fromIntegral timeoutUs)
  let nInt = fromIntegral n
  if nInt > 0
    then do
      vec <- V.unsafeFreeze mvec
      return $ Right (V.take (nInt * 2) vec)
    else return $ Left nInt
