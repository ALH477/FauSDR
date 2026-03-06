/*
 * faust_bridge.cpp — implementation of faust_bridge.h
 *
 * This translation unit #includes the Faust-generated modulator_gen.cpp
 * (which defines `mydsp`) and wraps it in C-callable functions.
 *
 * Compile:
 *   clang++ -std=c++17 -O2 -fPIC \
 *     -I../build -I${FAUST_ARCH_PATH} \
 *     -c faust_bridge.cpp -o faust_bridge.o
 *
 * The generated modulator_gen.cpp must be compiled first:
 *   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
 *         ../dsp/modulator_hs.dsp -o ../build/modulator_gen.cpp
 *
 * DeMoD LLC
 */

// Pull in the Faust-generated DSP class.
// faust -a produces a file that defines `mydsp` and includes faust/dsp.h.
#include "../../build/modulator_gen.cpp"

// MapUI lets us set parameters by OSC path string.
#include <faust/gui/MapUI.h>

#include "faust_bridge.h"

#include <cstdlib>
#include <cstring>

// ── Internal state bundled per handle ────────────────────────────────────────

struct FaustBridge {
    mydsp  dsp;
    MapUI  ui;
};

// ── API implementation ────────────────────────────────────────────────────────

extern "C" {

FaustDspHandle faust_dsp_create(void) {
    auto* b = new FaustBridge();
    b->dsp.buildUserInterface(&b->ui);
    return static_cast<FaustDspHandle>(b);
}

void faust_dsp_init(FaustDspHandle handle, int sample_rate) {
    auto* b = static_cast<FaustBridge*>(handle);
    b->dsp.init(sample_rate);
}

void faust_dsp_compute(FaustDspHandle handle,
                       int            count,
                       float**        inputs,
                       float**        outputs)
{
    auto* b = static_cast<FaustBridge*>(handle);
    b->dsp.compute(count,
                   reinterpret_cast<FAUSTFLOAT**>(inputs),
                   reinterpret_cast<FAUSTFLOAT**>(outputs));
}

int faust_dsp_num_inputs(FaustDspHandle handle) {
    return static_cast<FaustBridge*>(handle)->dsp.getNumInputs();
}

int faust_dsp_num_outputs(FaustDspHandle handle) {
    return static_cast<FaustBridge*>(handle)->dsp.getNumOutputs();
}

void faust_dsp_set_param(FaustDspHandle handle,
                          const char*    path,
                          float          value)
{
    auto* b = static_cast<FaustBridge*>(handle);
    b->ui.setParamValue(path, static_cast<FAUSTFLOAT>(value));
}

void faust_dsp_destroy(FaustDspHandle handle) {
    delete static_cast<FaustBridge*>(handle);
}

} // extern "C"
