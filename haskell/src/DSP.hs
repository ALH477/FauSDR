-- DCF.Faust.DSP — safe, high-level Faust DSP handle
-- DeMoD LLC
--
-- Wraps the raw FFI with:
--   - Resource safety via bracket / withDsp
--   - Typed channel count assertions at init time
--   - Vector-based compute with correct pointer lifetimes
--   - Named parameter setting via setParam
--
-- BUG FIX (vs previous): V.unsafeWith pins a vector only for the duration
-- of its callback. The previous implementation stored raw pointers after
-- those callbacks returned — undefined behaviour (GC can move vectors).
-- Fixed by nesting all unsafeWith calls so every pinned buffer is live
-- across the entire faustCompute call.

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

import Control.Exception        (bracket, throwIO, ErrorCall (..))
import Control.Monad            (when, forM)
import Data.Vector.Storable     (Vector)
import qualified Data.Vector.Storable as V
import Foreign.C.String         (withCString)
import Foreign.C.Types          (CFloat (..), CInt (..))
import Foreign.Marshal.Array    (allocaArray, pokeElemOff)
import Foreign.Ptr              (Ptr, castPtr, nullPtr)
import Foreign.Storable         (Storable)

import DCF.Faust.FFI

-- ── Config ─────────────────────────────────────────────────────────────────────

data DspConfig = DspConfig
  { dspSampleRate    :: Int   -- ^ samples/sec (e.g. 2_000_000)
  , dspExpectInputs  :: Int   -- ^ asserted against Faust getNumInputs()
  , dspExpectOutputs :: Int   -- ^ asserted against Faust getNumOutputs()
  } deriving (Show, Eq)

defaultDspConfig :: DspConfig
defaultDspConfig = DspConfig
  { dspSampleRate    = 2_000_000
  , dspExpectInputs  = 1   -- symbol stream
  , dspExpectOutputs = 2   -- I, Q
  }

-- ── Handle ─────────────────────────────────────────────────────────────────────

newtype DspHandle = DspHandle { _rawDsp :: FaustDsp }

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

    action (DspHandle raw)

-- ── Compute ───────────────────────────────────────────────────────────────────
--
-- Pointer safety: every vector must stay pinned (via V.unsafeWith) across the
-- entire faustCompute call. We achieve this by nesting the unsafeWith callbacks
-- before building the pointer arrays, so all pins are live simultaneously.
--
-- withPinnedPtrs :: [Vector Float] -> ([Ptr CFloat] -> IO a) -> IO a
-- threads through a continuation collecting pinned pointers.

withPinnedPtrs :: [Vector Float] -> ([Ptr CFloat] -> IO a) -> IO a
withPinnedPtrs []     k = k []
withPinnedPtrs (v:vs) k =
  V.unsafeWith v $ \p ->
    withPinnedPtrs vs $ \ps ->
      k (castPtr p : ps)

compute :: DspHandle
        -> [Vector Float]  -- ^ input channels (one per Faust input)
        -> Int             -- ^ block size in samples
        -> IO [Vector Float]
compute (DspHandle raw) inputs blockSize = do
  let nIn  = length inputs
      nOut = 2   -- I, Q — matches modulator_hs.dsp getNumOutputs()

  -- Allocate output buffers (mutable, 0-initialised)
  outVecs <- mapM (\_ -> V.replicate blockSize (0.0 :: Float)) [1..nOut]

  -- Pin all input buffers simultaneously, then output buffers,
  -- then build the C-level pointer arrays and call compute.
  withPinnedPtrs inputs  $ \inPtrs  ->
  withPinnedPtrs outVecs $ \outPtrs ->
    allocaArray nIn  $ \(inArr  :: Ptr (Ptr CFloat)) ->
    allocaArray nOut $ \(outArr :: Ptr (Ptr CFloat)) -> do
      mapM_ (\(i,p) -> pokeElemOff inArr  i p) (zip [0..] inPtrs)
      mapM_ (\(i,p) -> pokeElemOff outArr i p) (zip [0..] outPtrs)
      faustCompute raw (fromIntegral blockSize)
                   (castPtr inArr)
                   (castPtr outArr)

  return outVecs

-- ── Parameter control ─────────────────────────────────────────────────────────

setParam :: DspHandle -> String -> Float -> IO ()
setParam (DspHandle raw) path val =
  withCString path $ \cpath ->
    faustSetParam raw cpath (CFloat val)

-- ── Introspection ─────────────────────────────────────────────────────────────

numInputs :: DspHandle -> IO Int
numInputs (DspHandle raw) = fromIntegral <$> faustNumInputs raw

numOutputs :: DspHandle -> IO Int
numOutputs (DspHandle raw) = fromIntegral <$> faustNumOutputs raw
