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

// ── Root-raised-cosine FIR pulse shaping filter ───────────────────────────────
//
// 63-tap Kaiser-windowed RRC, alpha=0.35, beta=8.0, T=1 (normalized).
// Generated from the erf-based RRC formula (Proakis & Manolakis, Table 9-2):
//
//   h(t) = { (1 - α + 4α/π) / T                               t = 0
//          { α/(T√2) · [(1+2/π)sin(π/4α) + (1-2/π)cos(π/4α)] t = ±T/4α
//          { [sin(πt/T(1-α)) + 4αt/T·cos(πt/T(1+α))] /        otherwise
//            [πt/T(1-(4αt/T)²)] }
//
// Window: Kaiser with β=8.0 → ≈ −80 dB stopband attenuation.
// Coefficients are normalized so DC gain = 1.0.
//
// The `rrc_rolloff` hslider is retained as a parameter label for the MapUI
// so Haskell can read it back, but changes require a recompile to regenerate
// the coefficient table.  The embedded coefficients implement alpha=0.35.
//
// Delay: (63-1)/2 = 31 samples = 31/sps symbols (sps=208 @ 2MSPS/9600baud).
//
// fi.fir(coeffs, N, x): direct-form FIR convolution, Faust stdlib.
// waveform{} creates a read-only float table; int_part(i) indexes into it.

rrc35_coeffs = waveform {
  -0.000000499, 0.000000032, 0.000002364, -0.000003902, -0.000001013, 0.000011437, -0.000013271,
  -0.000007479, 0.000037337, -0.000032618, -0.000030961, 0.000099169, -0.000064865, -0.000098284,
  0.000231999, -0.000107568, -0.000269222, 0.000503715, -0.000143010, -0.000685631, 0.001068726,
  -0.000109512, -0.001750678, 0.002395231, 0.000215126, -0.005085017, 0.006802846, 0.001919179,
  -0.024575477, 0.056235955, -0.084361170, 1.095634118, -0.084361170, 0.056235955, -0.024575477,
  0.001919179, 0.006802846, -0.005085017, 0.000215126, 0.002395231, -0.001750678, -0.000109512,
  0.001068726, -0.000685631, -0.000143010, 0.000503715, -0.000269222, -0.000107568, 0.000231999,
  -0.000098284, -0.000064865, 0.000099169, -0.000030961, -0.000032618, 0.000037337, -0.000007479,
  -0.000013271, 0.000011437, -0.000001013, -0.000003902, 0.000002364, 0.000000032, -0.000000499
};

// Read the nth FIR coefficient from the waveform table (0-indexed).
rrc_coeff(n) = rrc35_coeffs, int(n) : rdtable;

// 63-tap direct-form FIR convolution via par/sum.
// Faust's fi.fir requires coefficients as a signal, so we use the
// explicit par/sum expansion referencing the waveform table.
rrc_ntaps = 63;
pulse_shape(sym) = sum(i, rrc_ntaps, rrc_coeff(i) * (sym@i));

// ── NCO — quadrature carrier pair ────────────────────────────────────────────
carrier_i = os.oscrc(carrier_freq);   // cosine
carrier_q = os.oscs(carrier_freq);    // sine (90° shifted)

// ── Phase modulation ──────────────────────────────────────────────────────────
// phi(t) = shaped_symbol(t) * mod_index
// I(t) = cos(wt + phi) = cos(wt)cos(phi) - sin(wt)sin(phi)
// Q(t) = sin(wt + phi) = cos(wt)sin(phi) + sin(wt)cos(phi)
pm_modulate(sym) = (i_mod * output_gain), (q_mod * output_gain)
with {
    phi   = pulse_shape(sym) * mod_index;
    i_mod = carrier_i * cos(phi) - carrier_q * sin(phi);
    q_mod = carrier_i * sin(phi) + carrier_q * cos(phi);
};

// ── Main process: 1 input (symbol stream) → 2 outputs (I, Q) ────────────────
process = pm_modulate;
