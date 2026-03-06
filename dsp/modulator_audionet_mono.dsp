// modulator_audionet_mono.dsp — AudioNet Mono Dial-Up Style Modulator
//
// Single channel BPSK modulator optimized for mono audio hardware
// Classic dial-up sounds with variable rates
//
// 1 input  (BPSK symbol stream ±1.0 from Haskell)
// 2 outputs (I, Q) for audio interface (mono operation)

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Sample rate selection for different audio quality levels
sample_rate = hslider("/audionet_mono/sample_rate[unit:Hz]", 48000, 44100, 192000, 1000) : si.smoo;

// Carrier frequency for audible modem sounds (1-4kHz range)
carrier_freq = hslider("/audionet_mono/carrier_freq[unit:Hz]", 2200, 1000, 4000, 10) : si.smoo;

// Symbol rate for variable speed transmission (1200-19200 baud)
symbol_rate = hslider("/audionet_mono/symbol_rate[unit:Sym/s]", 2400, 1200, 19200, 100) : si.smoo;

// Modulation index for classic dial-up sound character
mod_index = hslider("/audionet_mono/mod_index", 3.14159, 0, 6.28318, 0.001) : si.smoo;

// RRC rolloff for pulse shaping (optimized for audio bandwidth)
rrc_rolloff = hslider("/audionet_mono/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;

// Output gain control
output_gain = hslider("/audionet_mono/output_gain", 0.8, 0.0, 1.0, 0.001);

// ── Preset Buttons for Classic Dial-Up Experience ───────────────────────────
// Sound profile presets
deep_bbs = button("/audionet_mono/presets/deep_bbs") : select(1800);
classic_modem = button("/audionet_mono/presets/classic_modem") : select(2200);
high_speed = button("/audionet_mono/presets/high_speed") : select(2800);
ultra_clear = button("/audionet_mono/presets/ultra_clear") : select(3500);

// Rate presets
rate_1200 = button("/audionet_mono/presets/1200_baud") : select(1200);
rate_2400 = button("/audionet_mono/presets/2400_baud") : select(2400);
rate_9600 = button("/audionet_mono/presets/9600_baud") : select(9600);
rate_19200 = button("/audionet_mono/presets/19200_baud") : select(19200);

// ── RRC-approximating pulse shaping optimized for audio bandwidth ───────────
pulse_shape(sym) = sym : fi.lowpass(4, symbol_rate * (1.0 + rrc_rolloff) * 0.5);

// ── Quadrature NCO carrier for mono channel ─────────────────────────────────
carrier_i = os.osc(carrier_freq);          // cosine
carrier_q = os.osc(carrier_freq + 0.25);   // sine (+90°)

// ── Phase modulation for mono channel ───────────────────────────────────────
pm_modulate(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym) * mod_index;
    i_mod = carrier_i * cos(phi) - carrier_q * sin(phi);
    q_mod = carrier_i * sin(phi) + carrier_q * cos(phi);
};

// ── Main process (mono operation) ───────────────────────────────────────────
process = pm_modulate;