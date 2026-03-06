// modulator.dsp — DeMoD Faust SDR Modulator
//
// Produces 2-channel output: channel 0 = I, channel 1 = Q
// No audio input — autonomous baseband generator driven by internal state.
//
// Pipeline:
//   NCO (carrier) → phase modulator → root-raised cosine pulse shaper → I/Q split
//
// Controls (OSC/MIDI mappable):
//   /modulator/carrier_freq   — normalized carrier (0..1, maps to 0..SR/2)
//   /modulator/symbol_rate    — symbols per second
//   /modulator/mod_index      — modulation depth (0..pi for BPSK, 0..2pi for full PM)
//   /modulator/rrc_rolloff    — root-raised-cosine rolloff α (0.1..0.9)

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
carrier_freq  = hslider("/modulator/carrier_freq[unit:Hz]",  100000, 1000, 500000, 1)   : si.smoo;
symbol_rate   = hslider("/modulator/symbol_rate[unit:Sym/s]", 9600,   300, 115200, 1)   : si.smoo;
mod_index     = hslider("/modulator/mod_index",               3.14159, 0,  6.28318, 0.001) : si.smoo;
rrc_rolloff   = hslider("/modulator/rrc_rolloff",             0.35,   0.1, 0.9,    0.01)   : si.smoo;

// ── Symbol clock (square wave at symbol_rate) ─────────────────────────────────
//    Drives the data source — replace with actual frame bit extractor.
symbol_clock  = os.lf_squarewave(symbol_rate);

// ── BPSK symbol mapper: clock → {-1, +1} ─────────────────────────────────────
//    Trivial: positive clock half = +1, negative = -1
bpsk_symbol   = select2(symbol_clock > 0, -1.0, 1.0);

// ── Root-raised-cosine FIR pulse shaping filter ───────────────────────────────
//
// 63-tap Kaiser-windowed RRC, alpha=0.35, beta=8.0, T=1 (normalized).
// Replaces the 4th-order Butterworth approximation.
//
// The `rrc_rolloff` hslider is retained for MapUI compatibility.
// Coefficient changes require recompile (regenerate via rrc_gen.py).
//
// See modulator_hs.dsp for full derivation notes.

rrc35_coeffs = waveform {
  -0.000000499, 0.000000032, 0.000002364, -0.000003902, -0.000001013, 0.000011437, -0.000013271,
  -0.000007479, 0.000037337, -0.000032618, -0.000030961, 0.000099169, -0.000064865, -0.000098284,
  0.000231999, -0.000107568, -0.000269222, 0.000503715, -0.000143010, -0.000685631, 0.001068726,
  -0.000109512, -0.001750678, 0.002395231, 0.000215126, -0.005085017, 0.006802846, 0.001919179,
  -0.024575477, 0.056235955, -0.084361170, 1.095634118, -0.084361170, 0.056235955, -0.024575477,
  0.001919179, 0.006802846, -0.005085017, 0.000215126, 0.002395231, -0.001750678, -0.000109512,
  0.001068726, -0.000685631, -0.000143010, 0.000503715, -0.000269222, -0.000107568, 0.000231999,
  -0.000098284, -0.000064865, 0.000099169, -0.000030961, -0.000032618, 0.000037337, -0.000007479,
  -0.000013271, 0.000011437, -0.000001013, -0.000003902, 0.000002364, 0.000000032, -0.000000499,
};

rrc_coeff(n) = rrc35_coeffs, int(n) : rdtable;
rrc_ntaps    = 63;
pulse        = bpsk_symbol <: sum(i, rrc_ntaps, rrc_coeff(i) * (_@i));

// ── NCO: generates quadrature carrier ─────────────────────────────────────────
carrier_i = os.oscrc(carrier_freq);
carrier_q = os.oscs(carrier_freq);     // 90° phase-shifted sine

// ── Phase modulation: rotate carrier by pulse-shaped symbol phase ─────────────
//    PM: I(t) = cos(ωt + φ(t)),  Q(t) = sin(ωt + φ(t))
//    Implemented via trigonometric expansion:
//      I_mod = I·cos(φ) − Q·sin(φ)
//      Q_mod = I·sin(φ) + Q·cos(φ)
phi   = pulse * mod_index;
i_out = carrier_i * cos(phi) - carrier_q * sin(phi);
q_out = carrier_i * sin(phi) + carrier_q * cos(phi);

// ── Gain / normalization ─────────────────────────────────────────────────────
output_gain = 0.8;   // headroom below 1.0 to avoid DAC clipping

// ── Main process: 0-input, 2-output (I, Q) ───────────────────────────────────
process = (i_out * output_gain), (q_out * output_gain);
