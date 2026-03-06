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

// ── Root-raised cosine approximation (4th-order Butterworth lowpass) ─────────
//    True RRC requires precomputed FIR coefficients (see ITU-T rec. G.703).
//    This Butterworth approximation captures the spectral shape at the cost
//    of non-ideal stopband attenuation; adequate for lab bring-up and
//    spectral mask evaluation but should be replaced before FCC §15.249
//    submission with a proper FIR (e.g., 63-tap erf-based Kaiser-windowed RRC).
rrc_bw  = symbol_rate / ma.SR;
pulse   = bpsk_symbol : fi.lowpass(4, symbol_rate * (1.0 + rrc_rolloff) * 0.5);

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
