-- DCF.SDR.SoapyFFI — raw foreign imports for the SoapySDR C bridge
-- DeMoD LLC

{-# LANGUAGE ForeignFunctionInterface #-}

module DCF.SDR.SoapyFFI
  ( SoapyDev
  , SoapyStream
  , soapyOpen
  , soapyConfigureTx
  , soapyOpenTxStream
  , soapyWriteCF32
  , soapyCloseStream
  , soapyClose
  ) where

import Foreign.Ptr       (Ptr)
import Foreign.C.Types   (CDouble (..), CFloat (..), CInt (..), CLong (..))
import Foreign.C.String  (CString)

-- ── Opaque handles ────────────────────────────────────────────────────────────

data SoapyDevT
data SoapyStreamT

type SoapyDev    = Ptr SoapyDevT
type SoapyStream = Ptr SoapyStreamT

-- ── Imports ───────────────────────────────────────────────────────────────────

foreign import ccall unsafe "soapy_bridge.h soapy_open"
  soapyOpen :: CString -> IO SoapyDev

foreign import ccall unsafe "soapy_bridge.h soapy_configure_tx"
  soapyConfigureTx :: SoapyDev
                   -> CDouble   -- center_freq_hz
                   -> CDouble   -- sample_rate_hz
                   -> CDouble   -- gain_db
                   -> IO ()

foreign import ccall unsafe "soapy_bridge.h soapy_open_tx_stream"
  soapyOpenTxStream :: SoapyDev -> IO SoapyStream

-- | Write interleaved CF32 samples. Returns samples written or negative error.
foreign import ccall unsafe "soapy_bridge.h soapy_write_cf32"
  soapyWriteCF32 :: SoapyDev
                 -> SoapyStream
                 -> Ptr CFloat   -- iq_interleaved buffer
                 -> CInt         -- num_samples
                 -> CLong        -- timeout_us
                 -> IO CInt

foreign import ccall unsafe "soapy_bridge.h soapy_close_stream"
  soapyCloseStream :: SoapyDev -> SoapyStream -> IO ()

foreign import ccall unsafe "soapy_bridge.h soapy_close"
  soapyClose :: SoapyDev -> IO ()
