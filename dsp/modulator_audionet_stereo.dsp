// modulator_audionet_stereo.dsp — AudioNet Stereo Dial-Up Style Modulator
//
// Dual channel BPSK modulator for stereo audio hardware
// Higher throughput with separate left/right channels
//
// 2 inputs  (BPSK symbol streams ±1.0 from Haskell - left and right)
// 4 outputs (I, Q for left and right channels)

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Sample rate selection for different audio quality levels
sample_rate = hslider("/audionet_stereo/sample_rate[unit:Hz]", 96000, 44100, 192000, 1000) : si.smoo;

// Primary carrier frequency for left channel (1-4kHz range)
carrier_freq_left = hslider("/audionet_stereo/carrier_freq_left[unit:Hz]", 2200, 1000, 4000, 10) : si.smoo;

// Secondary carrier frequency for right channel
carrier_freq_right = hslider("/audionet_stereo/carrier_freq_right[unit:Hz]", 2600, 1000, 4000, 10) : si.smoo;

// Symbol rates for each channel (1200-38400 baud)
symbol_rate_left = hslider("/audionet_stereo/symbol_rate_left[unit:Sym/s]", 9600, 1200, 38400, 100) : si.smoo;
symbol_rate_right = hslider("/audionet_stereo/symbol_rate_right[unit:Sym/s]", 4800, 1200, 38400, 100) : si.smoo;

// Modulation index for classic dial-up sound character
mod_index = hslider("/audionet_stereo/mod_index", 3.14159, 0, 6.28318, 0.001) : si.smoo;

// RRC rolloff for pulse shaping (optimized for audio bandwidth)
rrc_rolloff = hslider("/audionet_stereo/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;

// Output gain control
output_gain = hslider("/audionet_stereo/output_gain", 0.8, 0.0, 1.0, 0.001);

// Channel independence mode (0=linked, 1=independent)
independent_channels = hslider("/audionet_stereo/independent_channels", 1, 0, 1, 1);

// ── Preset Buttons for Classic Dial-Up Experience ───────────────────────────
// Sound profile presets
deep_bbs = button("/audionet_stereo/presets/deep_bbs") : select(1800);
classic_modem = button("/audionet_stereo/presets/classic_modem") : select(2200);
high_speed = button("/audionet_stereo/presets/high_speed") : select(2800);
ultra_clear = button("/audionet_stereo/presets/ultra_clear") : select(3500);

// Rate presets
rate_1200 = button("/audionet_stereo/presets/1200_baud") : select(1200);
rate_2400 = button("/audionet_stereo/presets/2400_baud") : select(2400);
rate_9600 = button("/audionet_stereo/presets/9600_baud") : select(9600);
rate_19200 = button("/audionet_stereo/presets/19200_baud") : select(19200);
rate_38400 = button("/audionet_stereo/presets/38400_baud") : select(38400);

// ── RRC-approximating pulse shaping optimized for audio bandwidth ───────────
pulse_shape(sym, rate) = sym : fi.lowpass(4, rate * (1.0 + rrc_rolloff) * 0.5);

// ── Quadrature NCO carriers for both channels ───────────────────────────────
// Left channel carrier
carrier_i_left = os.osc(carrier_freq_left);          // cosine
carrier_q_left = os.osc(carrier_freq_left + 0.25);   // sine (+90°)

// Right channel carrier
carrier_i_right = os.osc(carrier_freq_right);          // cosine
carrier_q_right = os.osc(carrier_freq_right + 0.25);   // sine (+90°)

// ── Phase modulation for left channel ───────────────────────────────────────
pm_modulate_left(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym, symbol_rate_left) * mod_index;
    i_mod = carrier_i_left * cos(phi) - carrier_q_left * sin(phi);
    q_mod = carrier_i_left * sin(phi) + carrier_q_left * cos(phi);
};

// ── Phase modulation for right channel ──────────────────────────────────────
pm_modulate_right(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym, symbol_rate_right) * mod_index;
    i_mod = carrier_i_right * cos(phi) - carrier_q_right * sin(phi);
    q_mod = carrier_i_right * sin(phi) + carrier_q_right * cos(phi);
};

// ── Channel linking logic ───────────────────────────────────────────────────
// When independent_channels=0, right channel mirrors left with offset
linked_right_carrier_i = os.osc(carrier_freq_left + 400);
linked_right_carrier_q = os.osc(carrier_freq_left + 400 + 0.25);

pm_modulate_linked_right(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym, symbol_rate_left) * mod_index;
    i_mod = linked_right_carrier_i * cos(phi) - linked_right_carrier_q * sin(phi);
    q_mod = linked_right_carrier_i * sin(phi) + linked_right_carrier_q * cos(phi);
};

// ── Main process with channel selection ─────────────────────────────────────
process = pm_modulate_left : _, select2(independent_channels, pm_modulate_linked_right, pm_modulate_right);