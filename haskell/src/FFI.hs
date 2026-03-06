-- DCF.Faust.FFI — raw foreign imports for the Faust C bridge
-- DeMoD LLC
--
-- These are the lowest-level bindings. Prefer DCF.Faust.DSP for
-- safe, resource-managed usage.

{-# LANGUAGE ForeignFunctionInterface #-}

module DCF.Faust.FFI
  ( FaustDsp
  , faustCreate
  , faustInit
  , faustCompute
  , faustNumInputs
  , faustNumOutputs
  , faustSetParam
  , faustDestroy
  ) where

import Foreign.Ptr          (Ptr, nullPtr)
import Foreign.C.Types      (CInt (..), CFloat (..))
import Foreign.C.String     (CString)

-- ── Opaque handle ─────────────────────────────────────────────────────────────

-- | Opaque pointer to a FaustBridge struct (mydsp + MapUI).
data FaustDspT
type FaustDsp = Ptr FaustDspT

-- ── Raw imports ───────────────────────────────────────────────────────────────

foreign import ccall unsafe "faust_bridge.h faust_dsp_create"
  faustCreate :: IO FaustDsp

foreign import ccall unsafe "faust_bridge.h faust_dsp_init"
  faustInit :: FaustDsp -> CInt -> IO ()

-- | compute(dsp, count, inputs**, outputs**)
-- inputs and outputs are Ptr (Ptr CFloat) — array of channel pointers.
foreign import ccall unsafe "faust_bridge.h faust_dsp_compute"
  faustCompute :: FaustDsp
               -> CInt                    -- count
               -> Ptr (Ptr CFloat)        -- inputs  (array of channel ptrs)
               -> Ptr (Ptr CFloat)        -- outputs (array of channel ptrs)
               -> IO ()

foreign import ccall unsafe "faust_bridge.h faust_dsp_num_inputs"
  faustNumInputs :: FaustDsp -> IO CInt

foreign import ccall unsafe "faust_bridge.h faust_dsp_num_outputs"
  faustNumOutputs :: FaustDsp -> IO CInt

foreign import ccall unsafe "faust_bridge.h faust_dsp_set_param"
  faustSetParam :: FaustDsp -> CString -> CFloat -> IO ()

foreign import ccall unsafe "faust_bridge.h faust_dsp_destroy"
  faustDestroy :: FaustDsp -> IO ()
