/*
 * soapy-sdr-lib.cpp — DeMoD Faust Architecture (library variant, no main())
 *
 * Use this arch when compiling DSP files that will be linked into a shared
 * library or the Haskell/C bridge rather than a standalone executable.
 *
 * Compared to soapy-sdr.cpp:
 *   - No main() / argparse / signal handlers
 *   - No SoapySDR open/configure calls
 *   - Exposes only the Faust-generated `mydsp` class and the standard
 *     Faust C++ interface (init, compute, getNumInputs, getNumOutputs,
 *     buildUserInterface)
 *
 * The caller (faust_bridge.cpp or a test harness) is responsible for:
 *   - Allocating and initialising the mydsp instance
 *   - Providing correctly-sized input/output float** buffers
 *   - Opening / writing to the SDR hardware
 *
 * Build (via CMake faust_compile() helper):
 *   faust -a arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
 *         dsp/<scheme>.dsp -o build/<scheme>_gen.cpp
 *
 * DeMoD LLC — GPL-3.0
 */

// ── Faust boilerplate required by every arch file ─────────────────────────────
// These macros suppress parts of the standard Faust arch that pull in GUI
// toolkits or audio drivers we don't need.

#define FAUSTFLOAT float

// Minimal UI stub — MapUI is the only UI class Faust uses for parameter
// access via buildUserInterface().  We include a stripped version here.
#include <string>
#include <map>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

// Provide the mandatory Faust meta/UI base classes before <<includeclass>>.
struct Meta {
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

struct UI {
    virtual ~UI() {}
    virtual void openTabBox(const char* label)        = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label)   = 0;
    virtual void closeBox()                           = 0;
    virtual void addButton(const char* label, FAUSTFLOAT* zone)                              = 0;
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)                         = 0;
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone,
                                   FAUSTFLOAT init, FAUSTFLOAT min,
                                   FAUSTFLOAT max, FAUSTFLOAT step)                          = 0;
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone,
                                     FAUSTFLOAT init, FAUSTFLOAT min,
                                     FAUSTFLOAT max, FAUSTFLOAT step)                        = 0;
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone,
                             FAUSTFLOAT init, FAUSTFLOAT min,
                             FAUSTFLOAT max, FAUSTFLOAT step)                                = 0;
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone,
                                       FAUSTFLOAT min, FAUSTFLOAT max)                       = 0;
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone,
                                     FAUSTFLOAT min, FAUSTFLOAT max)                         = 0;
    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)                 = 0;
};

// ── MapUI — flat string→FAUSTFLOAT* parameter map ────────────────────────────
// faust_bridge.cpp uses this to implement faust_dsp_set_param().

class MapUI : public UI {
public:
    std::map<std::string, FAUSTFLOAT*> params;

    void openTabBox(const char*)        override {}
    void openHorizontalBox(const char*) override {}
    void openVerticalBox(const char*)   override {}
    void closeBox()                     override {}

    void addButton(const char* label, FAUSTFLOAT* z) override
        { params[label] = z; }
    void addCheckButton(const char* label, FAUSTFLOAT* z) override
        { params[label] = z; }
    void addVerticalSlider(const char* label, FAUSTFLOAT* z, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override
        { params[label] = z; }
    void addHorizontalSlider(const char* label, FAUSTFLOAT* z, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override
        { params[label] = z; }
    void addNumEntry(const char* label, FAUSTFLOAT* z, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) override
        { params[label] = z; }
    void addHorizontalBargraph(const char*, FAUSTFLOAT*, FAUSTFLOAT, FAUSTFLOAT) override {}
    void addVerticalBargraph(const char*, FAUSTFLOAT*, FAUSTFLOAT, FAUSTFLOAT)   override {}
    void declare(FAUSTFLOAT*, const char*, const char*) override {}

    bool setParam(const std::string& path, FAUSTFLOAT val) {
        auto it = params.find(path);
        if (it == params.end()) return false;
        *(it->second) = val;
        return true;
    }
};

// ── Faust-generated DSP class ─────────────────────────────────────────────────
// Injected by the Faust compiler when this file is used as an architecture.

<<includeIntrinsic>>
<<includeclass>>

// ── That's it — no main(), no SDR open, no signal handlers ───────────────────
// The Faust-generated `mydsp` class is now available for linking.
// See faust_bridge.cpp for the C wrapper that exposes it to Haskell.
