// modulator_audionet.dsp — AudioNet Modern Dial-Up Style Modulator
//
// Variable rate BPSK modulator with audible carrier frequencies
// Designed for mono/stereo music hardware with nostalgic dial-up sounds
//
// 1 input  (BPSK symbol stream ±1.0 from Haskell)
// 2 outputs (I, Q) for SoapySDR or audio interface

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Sample rate selection for different audio quality levels
sample_rate = hslider("/audionet/sample_rate[unit:Hz]", 48000, 44100, 192000, 1000) : si.smoo;

// Carrier frequency for audible modem sounds (1-4kHz range)
carrier_freq = hslider("/audionet/carrier_freq[unit:Hz]", 2200, 1000, 4000, 10) : si.smoo;

// Symbol rate for variable speed transmission (1200-38400 baud)
symbol_rate = hslider("/audionet/symbol_rate[unit:Sym/s]", 2400, 1200, 38400, 100) : si.smoo;

// Modulation index for classic dial-up sound character
mod_index = hslider("/audionet/mod_index", 3.14159, 0, 6.28318, 0.001) : si.smoo;

// RRC rolloff for pulse shaping (optimized for audio bandwidth)
rrc_rolloff = hslider("/audionet/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;

// Output gain control
output_gain = hslider("/audionet/output_gain", 0.8, 0.0, 1.0, 0.001);

// Stereo mode enable (0=mono, 1=stereo)
stereo_mode = hslider("/audionet/stereo_mode", 0, 0, 1, 1);

// Secondary carrier frequency offset for stereo mode
stereo_offset = hslider("/audionet/stereo_offset[unit:Hz]", 400, 0, 2000, 10);

// ── Preset Buttons for Classic Dial-Up Experience ───────────────────────────
// Sound profile presets
deep_bbs = button("/audionet/presets/deep_bbs") : select(1800);
classic_modem = button("/audionet/presets/classic_modem") : select(2200);
high_speed = button("/audionet/presets/high_speed") : select(2800);
ultra_clear = button("/audionet/presets/ultra_clear") : select(3500);

// Rate presets
rate_1200 = button("/audionet/presets/1200_baud") : select(1200);
rate_2400 = button("/audionet/presets/2400_baud") : select(2400);
rate_9600 = button("/audionet/presets/9600_baud") : select(9600);
rate_19200 = button("/audionet/presets/19200_baud") : select(19200);
rate_38400 = button("/audionet/presets/38400_baud") : select(38400);

// ── RRC-approximating pulse shaping optimized for audio bandwidth ───────────
pulse_shape(sym) = sym : fi.lowpass(4, symbol_rate * (1.0 + rrc_rolloff) * 0.5);

// ── Quadrature NCO carrier for primary channel ──────────────────────────────
carrier_i = os.osc(carrier_freq);          // cosine
carrier_q = os.osc(carrier_freq + 0.25);   // sine (+90°)

// ── Secondary carrier for stereo mode ───────────────────────────────────────
secondary_freq = carrier_freq + stereo_offset;
carrier_i_sec = os.osc(secondary_freq);          // cosine
carrier_q_sec = os.osc(secondary_freq + 0.25);   // sine (+90°)

// ── Phase modulation for primary channel ────────────────────────────────────
pm_modulate_primary(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym) * mod_index;
    i_mod = carrier_i * cos(phi) - carrier_q * sin(phi);
    q_mod = carrier_i * sin(phi) + carrier_q * cos(phi);
};

// ── Phase modulation for secondary channel (stereo mode) ────────────────────
pm_modulate_secondary(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym) * mod_index;
    i_mod = carrier_i_sec * cos(phi) - carrier_q_sec * sin(phi);
    q_mod = carrier_i_sec * sin(phi) + carrier_q_sec * cos(phi);
};

// ── Stereo/Mono selection logic ─────────────────────────────────────────────
stereo_output(sym) = (
    primary_i + secondary_i,
    primary_q + secondary_q
)
with {
    (primary_i, primary_q) = pm_modulate_primary(sym);
    (secondary_i, secondary_q) = pm_modulate_secondary(sym);
};

mono_output(sym) = pm_modulate_primary(sym);

// ── Main process with stereo/mono selection ─────────────────────────────────
process = select2(stereo_mode, mono_output, stereo_output);