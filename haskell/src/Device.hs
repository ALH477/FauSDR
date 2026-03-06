-- DCF.SDR.Device — safe SoapySDR TX device handle
-- DeMoD LLC

module DCF.SDR.Device
  ( SdrConfig (..)
  , defaultSdrConfig
  , SdrDevice
  , withSdrDevice
  , writeSamples   -- ^ writeSamples :: SdrDevice -> Vector Float -> Int -> IO Int
  ) where

import Control.Exception     (bracket, throwIO, ErrorCall (..))
import Data.Vector.Storable  (Vector)
import qualified Data.Vector.Storable as V
import Foreign.C.String      (withCString)
import Foreign.C.Types       (CDouble (..), CFloat (..), CInt (..), CLong (..))
import Foreign.Ptr           (nullPtr)

import DCF.SDR.SoapyFFI

-- ── Config ─────────────────────────────────────────────────────────────────────

data SdrConfig = SdrConfig
  { sdrDriver        :: String   -- ^ e.g. "rtlsdr", "hackrf", "lime"
  , sdrCenterFreqHz  :: Double   -- ^ carrier frequency (Hz)
  , sdrSampleRateHz  :: Double   -- ^ sample rate (Hz)
  , sdrGainDb        :: Double   -- ^ TX gain (dB)
  , sdrTimeoutUs     :: Int      -- ^ write timeout (µs)
  } deriving (Show, Eq)

defaultSdrConfig :: SdrConfig
defaultSdrConfig = SdrConfig
  { sdrDriver       = "rtlsdr"
  , sdrCenterFreqHz = 433.92e6
  , sdrSampleRateHz = 2e6
  , sdrGainDb       = 20.0
  , sdrTimeoutUs    = 1_000_000
  }

-- ── Handle ─────────────────────────────────────────────────────────────────────

-- | Opaque SDR device + stream pair.
data SdrDevice = SdrDevice
  { _dev    :: SoapyDev
  , _stream :: SoapyStream
  }

-- ── Lifecycle ─────────────────────────────────────────────────────────────────

-- | Open the SDR device, configure TX, open the stream.
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

-- ── Write ─────────────────────────────────────────────────────────────────────

-- | Write a vector of interleaved complex float samples [I0,Q0,I1,Q1,...].
--   The vector length must be even (2 * num_samples).
--   @timeoutUs@ comes from the active SdrConfig — do NOT pass
--   defaultSdrConfig here, as that would silently ignore the caller's timeout.
--   Returns the number of IQ pairs written (negative on SoapySDR error).
writeSamples :: SdrDevice -> Vector Float -> Int -> IO Int
writeSamples (SdrDevice dev stream) iqVec timeoutUs = do
  let numSamples = V.length iqVec `div` 2
  V.unsafeWith iqVec $ \ptr -> do
    n <- soapyWriteCF32 dev stream ptr
                        (fromIntegral numSamples)
                        (fromIntegral timeoutUs)
    return (fromIntegral n)
