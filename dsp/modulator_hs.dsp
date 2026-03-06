// modulator_hs.dsp — DeMoD Faust DSP for Haskell integration
//
// This version of the modulator accepts 1 INPUT (the symbol stream from Haskell)
// and produces 2 OUTPUTS (I, Q).
//
// Haskell feeds pre-encoded BPSK symbols as float samples into input channel 0.
// Faust handles:
//   1. RRC pulse shaping filter
//   2. Phase modulation onto the NCO carrier
//   3. Quadrature I/Q split
//
// The key change from modulator.dsp: `process` now takes 1 input.
// faust_bridge.cpp asserts getNumInputs()==1, getNumOutputs()==2.
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/modulator_hs.dsp -o build/modulator_gen.cpp

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── Parameters (settable via faust_dsp_set_param / MapUI) ────────────────────
carrier_freq  = hslider("/modulator/carrier_freq[unit:Hz]",  0.1, 0.001, 0.5, 0.0001) : si.smoo;
// Note: carrier_freq is now normalized (0..0.5 = 0..SR/2)
// Haskell sets this to: actual_hz / (sample_rate / 2)

symbol_rate   = hslider("/modulator/symbol_rate[unit:Sym/s]", 9600, 300, 115200, 1) : si.smoo;
mod_index     = hslider("/modulator/mod_index",  3.14159, 0, 6.28318, 0.001)        : si.smoo;
rrc_rolloff   = hslider("/modulator/rrc_rolloff", 0.35,   0.1, 0.9,   0.01)         : si.smoo;
output_gain   = hslider("/modulator/output_gain", 0.8,    0.0, 1.0,   0.001);

// ── RRC-approximating pulse shape filter ─────────────────────────────────────
// Input: symbol stream (float, ±1.0 per sample at upsampled rate)
// Apply lowpass at (symbol_rate * (1 + rolloff) / 2) to approximate RRC.
// Replace with a proper FIR coefficient table for production.
pulse_shape(sym) = sym : fi.lowpass(4, symbol_rate * (1.0 + rrc_rolloff) * 0.5);

// ── NCO — quadrature carrier pair ────────────────────────────────────────────
carrier_i = os.oscrc(carrier_freq);   // cosine
carrier_q = os.oscs(carrier_freq);    // sine (90° shifted)

// ── Phase modulation ──────────────────────────────────────────────────────────
// phi(t) = shaped_symbol(t) * mod_index
// I(t) = cos(wt + phi) = cos(wt)cos(phi) - sin(wt)sin(phi)
// Q(t) = sin(wt + phi) = cos(wt)sin(phi) + sin(wt)cos(phi)
pm_modulate(sym) =
  let {
    phi   = pulse_shape(sym) * mod_index;
    i_mod = carrier_i * cos(phi) - carrier_q * sin(phi);
    q_mod = carrier_i * sin(phi) + carrier_q * cos(phi);
  }
  in (i_mod * output_gain), (q_mod * output_gain);

// ── Main process: 1 input (symbol stream) → 2 outputs (I, Q) ────────────────
process = pm_modulate;
