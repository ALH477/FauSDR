/*
 * faust_bridge.cpp — C-callable wrapper around Faust-generated mydsp
 * DeMoD LLC | GPL-3.0
 *
 * This file is compiled as C++ by Cabal via the `cxx-sources` stanza.
 * It #includes the Faust-generated modulator_gen.cpp (which defines `mydsp`)
 * and wraps it in plain C functions that Haskell can call via FFI.
 *
 * BUG FIX: the previous version hard-coded "../../build/modulator_gen.cpp"
 * which broke every out-of-tree build (including `cabal build`, which puts
 * generated files under dist-newstyle/).  The generated file is now found
 * via the DEMOD_FAUST_GEN_CPP preprocessor macro, which CMake/Cabal must
 * define at compile time:
 *
 *   Cabal:  cxx-options: -DDEMOD_FAUST_GEN_CPP="<path>/modulator_gen.cpp"
 *   CMake:  target_compile_definitions(... -DDEMOD_FAUST_GEN_CPP=...)
 *
 * If DEMOD_FAUST_GEN_CPP is not defined (e.g. during a stub/test build),
 * the bridge compiles as a no-op stub with all functions returning defaults.
 * This lets the Haskell library build and link even before the Faust DSP
 * compilation step is run.
 *
 * Build flow:
 *   1. faust -a arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
 *            dsp/modulator_hs.dsp -o build/modulator_gen.cpp
 *   2. cabal build  (or cmake --build)
 *      → clang++ -std=c++17 -DDEMOD_FAUST_GEN_CPP='"build/modulator_gen.cpp"' \
 *                cbits/faust_bridge.cpp ...
 */

#ifdef DEMOD_FAUST_GEN_CPP
  // Include the Faust-generated C++ class (defines `mydsp`).
  // Using a macro-defined path allows any build system to inject the
  // correct absolute or relative path at compile time.
  #include DEMOD_FAUST_GEN_CPP
  #include <faust/gui/MapUI.h>
  #define FAUST_AVAILABLE 1
#else
  // Stub build: the Faust DSP file has not been compiled yet.
  // All bridge functions are no-ops; the linker will resolve them but
  // calling them at runtime without a real DSP will trigger DCFError.
  #warning "DEMOD_FAUST_GEN_CPP not defined — building faust_bridge as stub"
  #define FAUST_AVAILABLE 0
#endif

#include "faust_bridge.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

// ── Internal state ────────────────────────────────────────────────────────────

#if FAUST_AVAILABLE
struct FaustBridge {
    mydsp  dsp;
    MapUI  ui;
};
#else
struct FaustBridge {
    int stub;   // placeholder so sizeof is non-zero
};
#endif

// ── API implementation ────────────────────────────────────────────────────────

extern "C" {

FaustDspHandle faust_dsp_create(void) {
#if FAUST_AVAILABLE
    auto* b = new FaustBridge();
    b->dsp.buildUserInterface(&b->ui);
    return static_cast<FaustDspHandle>(b);
#else
    fprintf(stderr, "[faust-bridge] STUB: faust_dsp_create called without DSP\n");
    return nullptr;
#endif
}

void faust_dsp_init(FaustDspHandle handle, int sample_rate) {
#if FAUST_AVAILABLE
    if (!handle) return;
    static_cast<FaustBridge*>(handle)->dsp.init(sample_rate);
#else
    (void)handle; (void)sample_rate;
#endif
}

void faust_dsp_compute(FaustDspHandle handle,
                       int            count,
                       float**        inputs,
                       float**        outputs)
{
#if FAUST_AVAILABLE
    if (!handle) return;
    auto* b = static_cast<FaustBridge*>(handle);
    b->dsp.compute(count,
                   reinterpret_cast<FAUSTFLOAT**>(inputs),
                   reinterpret_cast<FAUSTFLOAT**>(outputs));
#else
    (void)handle; (void)count; (void)inputs; (void)outputs;
#endif
}

int faust_dsp_num_inputs(FaustDspHandle handle) {
#if FAUST_AVAILABLE
    if (!handle) return 0;
    return static_cast<FaustBridge*>(handle)->dsp.getNumInputs();
#else
    (void)handle; return 0;
#endif
}

int faust_dsp_num_outputs(FaustDspHandle handle) {
#if FAUST_AVAILABLE
    if (!handle) return 0;
    return static_cast<FaustBridge*>(handle)->dsp.getNumOutputs();
#else
    (void)handle; return 0;
#endif
}

void faust_dsp_set_param(FaustDspHandle handle,
                          const char*    path,
                          float          value)
{
#if FAUST_AVAILABLE
    if (!handle || !path) return;
    auto* b = static_cast<FaustBridge*>(handle);
    b->ui.setParamValue(path, static_cast<FAUSTFLOAT>(value));
#else
    (void)handle; (void)path; (void)value;
#endif
}

void faust_dsp_destroy(FaustDspHandle handle) {
#if FAUST_AVAILABLE
    delete static_cast<FaustBridge*>(handle);
#else
    (void)handle;
#endif
}

} // extern "C"
