/*
 * faust_bridge.h — C-callable wrapper for Faust-generated mydsp C++ class
 *
 * Haskell's FFI cannot call C++ methods directly. This bridge exposes
 * a flat C API that Haskell imports via foreign import ccall.
 *
 * Build note: this file is compiled as C++, included in dcf-faust-sdr.cabal
 * under the `cxx-sources` stanza.
 *
 * DeMoD LLC
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void* FaustDspHandle;

/** Allocate a new mydsp instance (calls operator new). */
FaustDspHandle faust_dsp_create(void);

/**
 * Initialize DSP at the given sample rate.
 * Must be called before faust_dsp_compute.
 */
void faust_dsp_init(FaustDspHandle dsp, int sample_rate);

/**
 * Run the DSP compute kernel.
 *
 * @param dsp      opaque DSP handle
 * @param count    number of samples per channel
 * @param inputs   array of `num_inputs` float* channel buffers (may be NULL if 0 inputs)
 * @param outputs  array of `num_outputs` float* channel buffers (caller-allocated)
 */
void faust_dsp_compute(FaustDspHandle dsp,
                       int            count,
                       float**        inputs,
                       float**        outputs);

/** Number of input channels declared in the Faust program. */
int faust_dsp_num_inputs(FaustDspHandle dsp);

/** Number of output channels declared in the Faust program. */
int faust_dsp_num_outputs(FaustDspHandle dsp);

/** Set a named parameter by its OSC-style path (e.g. "/modulator/carrier_freq"). */
void faust_dsp_set_param(FaustDspHandle dsp,
                          const char*    path,
                          float          value);

/** Free the DSP instance. */
void faust_dsp_destroy(FaustDspHandle dsp);

#ifdef __cplusplus
}
#endif
