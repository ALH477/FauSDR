// modulator_hs.dsp — DeMoD Faust DSP for Haskell integration
//
// 1 input  (BPSK symbol stream ±1.0 from Haskell)
// 2 outputs (I, Q) for SoapySDR

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
carrier_freq  = hslider("/modulator/carrier_freq[unit:Hz]", 0.1, 0.001, 0.5, 0.0001) : si.smoo;
symbol_rate   = hslider("/modulator/symbol_rate[unit:Sym/s]", 9600, 300, 115200, 1) : si.smoo;
mod_index     = hslider("/modulator/mod_index", 3.14159, 0, 6.28318, 0.001) : si.smoo;
rrc_rolloff   = hslider("/modulator/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;
output_gain   = hslider("/modulator/output_gain", 0.8, 0.0, 1.0, 0.001);

// ── RRC-approximating pulse shaping ─────────────────────────────────────────
pulse_shape(sym) = sym : fi.lowpass(4, symbol_rate * (1.0 + rrc_rolloff) * 0.5);

// ── Quadrature NCO carrier (portable & stable) ──────────────────────────────
carrier_i = os.osc(carrier_freq);          // cosine
carrier_q = os.osc(carrier_freq + 0.25);   // sine (+90°)

// ── Phase modulation ────────────────────────────────────────────────────────
pm_modulate(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym) * mod_index;
    i_mod = carrier_i * cos(phi) - carrier_q * sin(phi);
    q_mod = carrier_i * sin(phi) + carrier_q * cos(phi);
};

// ── Main process (exactly what faust_bridge.cpp expects) ────────────────────
process = pm_modulate;
