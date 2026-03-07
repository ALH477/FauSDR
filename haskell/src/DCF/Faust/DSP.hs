-- DCF.Faust.DSP — safe, high-level Faust DSP handle
-- DeMoD LLC
--
-- Wraps the raw FFI with:
--   - Resource safety via bracket / withDsp
--   - Typed channel count assertions at init time
--   - Vector-based compute with correct pointer lifetimes
--   - Named parameter setting via setParam
--
-- BUG FIXES vs previous version:
--   1. nOut was hardcoded to 2.  It now reads the actual output channel count
--      from the DSP handle at init time and stores it in DspHandle.
--   2. Output buffers were allocated as immutable V.Vector via V.replicate,
--      then written through raw pointers by Faust — undefined behaviour because
--      GHC may share or relocate the underlying array.  Fixed: allocate mutable
--      VM.MVector, pin via VM.unsafeWith, then freeze after compute returns.
--   3. faustCompute was declared `unsafe`.  Faust-generated C++ may call malloc
--      or throw std::exceptions (via the MapUI path); `unsafe` suppresses GHC's
--      ability to switch threads during the call and may deadlock with the GC
--      under -N.  Changed to `safe` in FFI; bridge header updated to match.
--      (See DCF.Faust.FFI for the corresponding change.)

{-# LANGUAGE ScopedTypeVariables #-}

module DCF.Faust.DSP
  ( DspHandle
  , DspConfig (..)
  , defaultDspConfig
  , withDsp
  , compute
  , setParam
  , numInputs
  , numOutputs
  ) where

import Control.Exception          (bracket, throwIO, ErrorCall (..))
import Control.Monad              (when, forM)
import Data.Vector.Storable       (Vector)
import qualified Data.Vector.Storable         as V
import qualified Data.Vector.Storable.Mutable as VM
import Foreign.C.String           (withCString)
import Foreign.C.Types            (CFloat (..), CInt (..))
import Foreign.Marshal.Array      (allocaArray)
import Foreign.Storable           (pokeElemOff)
import Foreign.Ptr                (Ptr, castPtr, nullPtr)

import DCF.Faust.FFI

-- ── Config ────────────────────────────────────────────────────────────────────

data DspConfig = DspConfig
  { dspSampleRate    :: Int   -- ^ samples/sec (e.g. 2_000_000)
  , dspExpectInputs  :: Int   -- ^ asserted against Faust getNumInputs()
  , dspExpectOutputs :: Int   -- ^ asserted against Faust getNumOutputs()
  } deriving (Show, Eq)

defaultDspConfig :: DspConfig
defaultDspConfig = DspConfig
  { dspSampleRate    = 2_000_000
  , dspExpectInputs  = 1   -- symbol stream in
  , dspExpectOutputs = 2   -- I, Q out
  }

-- ── Handle ────────────────────────────────────────────────────────────────────

-- | Opaque handle.  nIn/nOut are read from the DSP at init time and cached
--   so that compute does not need to call into C on every block.
data DspHandle = DspHandle
  { _rawDsp :: FaustDsp
  , _nIn    :: Int
  , _nOut   :: Int
  }

-- ── Lifecycle ─────────────────────────────────────────────────────────────────

withDsp :: DspConfig -> (DspHandle -> IO a) -> IO a
withDsp cfg action =
  bracket faustCreate faustDestroy $ \raw -> do
    when (raw == nullPtr) $
      throwIO (ErrorCall "faust_dsp_create returned NULL")
    faustInit raw (fromIntegral $ dspSampleRate cfg)

    ins  <- fromIntegral <$> faustNumInputs  raw
    outs <- fromIntegral <$> faustNumOutputs raw

    when (ins /= dspExpectInputs cfg) $
      throwIO $ ErrorCall $
        "Faust DSP: expected " <> show (dspExpectInputs cfg)
        <> " inputs, got " <> show ins

    when (outs /= dspExpectOutputs cfg) $
      throwIO $ ErrorCall $
        "Faust DSP: expected " <> show (dspExpectOutputs cfg)
        <> " outputs, got " <> show outs

    action (DspHandle raw ins outs)

-- ── Compute ───────────────────────────────────────────────────────────────────
--
-- Pointer safety invariant:
--   Every Vector/MVector must remain PINNED (via unsafeWith) across the
--   entire faustCompute call.  We achieve this by nesting all unsafeWith
--   callbacks BEFORE building the C-level pointer arrays, so all pins are
--   live simultaneously when faustCompute executes.
--
-- Mutable output buffers:
--   Faust writes into the output buffers through raw float** pointers.
--   We allocate mutable VM.MVector buffers, pin them, call compute, then
--   V.freeze to produce immutable output Vectors.  Using V.replicate (immutable)
--   and casting to Ptr was UB; this fixes that.

-- | Pin a list of immutable Vectors and collect their raw Ptr CFloat pointers.
withPinnedInputPtrs :: [Vector Float] -> ([Ptr CFloat] -> IO a) -> IO a
withPinnedInputPtrs []     k = k []
withPinnedInputPtrs (v:vs) k =
  V.unsafeWith v $ \p ->
    withPinnedInputPtrs vs $ \ps ->
      k (castPtr p : ps)

-- | Allocate mutable output buffers, pin them, run action, then freeze.
withMutableOutputs
  :: Int                                             -- nOut
  -> Int                                             -- blockSize
  -> ([Ptr CFloat] -> IO ())                         -- action receiving raw ptrs
  -> IO [Vector Float]
withMutableOutputs nOut blockSize action = do
  -- Allocate nOut mutable storable vectors, each of length blockSize
  mvecs <- mapM (\_ -> VM.new blockSize) [1 .. nOut]

  -- Pin them all simultaneously, collect raw ptrs, run the compute kernel
  let go []       ptrs = action (reverse ptrs)
      go (mv:mvs) ptrs =
        VM.unsafeWith mv $ \p ->
          go mvs (castPtr p : ptrs)
  go mvecs []

  -- Freeze into immutable Vectors (O(1) — no copy, just removes mutability)
  mapM V.unsafeFreeze mvecs

compute :: DspHandle
        -> [Vector Float]  -- ^ input channels (one per Faust input)
        -> Int             -- ^ block size in samples
        -> IO [Vector Float]
compute (DspHandle raw nIn nOut) inputs blockSize =
  withPinnedInputPtrs inputs $ \inPtrs ->
  withMutableOutputs nOut blockSize $ \outPtrs ->
    allocaArray nIn  $ \(inArr  :: Ptr (Ptr CFloat)) ->
    allocaArray nOut $ \(outArr :: Ptr (Ptr CFloat)) -> do
      mapM_ (\(i,p) -> pokeElemOff inArr  i p) (zip [0..] inPtrs)
      mapM_ (\(i,p) -> pokeElemOff outArr i p) (zip [0..] outPtrs)
      faustCompute raw (fromIntegral blockSize)
                   (castPtr inArr)
                   (castPtr outArr)

-- ── Parameter control ─────────────────────────────────────────────────────────

setParam :: DspHandle -> String -> Float -> IO ()
setParam (DspHandle raw _ _) path val =
  withCString path $ \cpath ->
    faustSetParam raw cpath (CFloat val)

-- ── Introspection ─────────────────────────────────────────────────────────────

numInputs :: DspHandle -> IO Int
numInputs (DspHandle raw _ _) = fromIntegral <$> faustNumInputs raw

numOutputs :: DspHandle -> IO Int
numOutputs (DspHandle raw _ _) = fromIntegral <$> faustNumOutputs raw
