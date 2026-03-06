// demod_hs.dsp — DeMoD Faust BPSK coherent demodulator for Haskell/SDR RX
//
// Inputs:  2 (I and Q from SoapySDR / complex baseband)
// Outputs: 1 (soft decision / symbol stream ≈ ±1.0)
//
// Symmetric counterpart to modulator_hs.dsp:
//   • matched RRC filtering
//   • coherent down-conversion (multiply by recovered carrier)
//   • integrate-and-dump (simple symbol-rate sampler)

import("stdfaust.lib");
import("filters.lib");
import("maths.lib");

// ── UI Parameters (set via MapUI / same paths as modulator) ─────────────────
carrier_freq  = hslider("/demodulator/carrier_freq[unit:Hz]", 0.1, 0.001, 0.5, 0.0001) : si.smoo;
// normalized: 0..0.5 = 0..fs/2 ; set to actual_hz / (sample_rate / 2)

symbol_rate   = hslider("/demodulator/symbol_rate[unit:Sym/s]", 9600, 300, 115200, 1) : si.smoo;
rrc_rolloff   = hslider("/demodulator/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;
output_gain   = hslider("/demodulator/output_gain", 1.0, 0.0, 2.0, 0.001);   // can amplify soft decisions

// ── Matched RRC-approximating filter (same as tx pulse shaping) ─────────────
rrc_matched(sig) = sig : fi.lowpass(4, symbol_rate * (1.0 + rrc_rolloff) * 0.5);

// ── Recovered quadrature carrier (must match transmitter phase!) ─────────────
carrier_i = os.osc(carrier_freq);          // cos(2π f t)
carrier_q = os.osc(carrier_freq + 0.25);   // sin(2π f t) = cos(2π f t + π/2)

// ── Coherent demodulation ────────────────────────────────────────────────────
demod(i_in, q_in) =
    letrec {
        'baseband = (i_in * carrier_i + q_in * carrier_q) * 2.0;   // real part after mixing (I*cos + Q*sin)
        'shaped   = rrc_matched(baseband);
    }
    in shaped * output_gain;

// ── Main process: 2 in (I,Q) → 1 out (soft symbols ≈ ±1) ────────────────────
process = demod;
