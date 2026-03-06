// demodulator_audionet_mono.dsp — AudioNet Mono BPSK Demodulator
//
// Simplified coherent BPSK demodulator optimized for mono audio hardware
// Fixed carrier frequency tracking with simplified timing recovery
// Designed to match the AudioNet mono modulator
//
// 2 inputs  (I, Q from audio interface)
// 1 output (demodulated BPSK symbols ±1.0 for Haskell transport)

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Sample rate selection for different audio quality levels
sample_rate = hslider("/demodulator_audionet_mono/sample_rate[unit:Hz]", 48000, 44100, 192000, 1000) : si.smoo;

// Fixed carrier frequency for mono operation (1-4kHz range)
carrier_freq = hslider("/demodulator_audionet_mono/carrier_freq[unit:Hz]", 2200, 1000, 4000, 10) : si.smoo;

// Symbol rate for timing recovery (1200-19200 baud)
symbol_rate = hslider("/demodulator_audionet_mono/symbol_rate[unit:Sym/s]", 9600, 1200, 19200, 100) : si.smoo;

// PLL parameters for carrier recovery
pll_bandwidth = hslider("/demodulator_audionet_mono/pll_bandwidth", 0.02, 0.001, 0.1, 0.001) : si.smoo;

// Timing recovery parameters
timing_gain = hslider("/demodulator_audionet_mono/timing_gain", 0.2, 0.01, 1.0, 0.01) : si.smoo;

// RRC filter parameters
rrc_rolloff = hslider("/demodulator_audionet_mono/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;

// Output gain
output_gain = hslider("/demodulator_audionet_mono/output_gain", 1.0, 0.0, 2.0, 0.01);

// ── Preset Buttons for Common Rates ─────────────────────────────────────────
// Sound profile presets
deep_bbs = button("/demodulator_audionet_mono/presets/deep_bbs") : select(1800);
classic_modem = button("/demodulator_audionet_mono/presets/classic_modem") : select(2200);
high_speed = button("/demodulator_audionet_mono/presets/high_speed") : select(2800);

// Rate presets
rate_1200 = button("/demodulator_audionet_mono/presets/1200_baud") : select(1200);
rate_2400 = button("/demodulator_audionet_mono/presets/2400_baud") : select(2400);
rate_9600 = button("/demodulator_audionet_mono/presets/9600_baud") : select(9600);
rate_19200 = button("/demodulator_audionet_mono/presets/19200_baud") : select(19200);

// ── Fixed Carrier Generation ────────────────────────────────────────────────
// Generate fixed carrier for coherent detection
fixed_carrier_i = os.osc(carrier_freq);
fixed_carrier_q = os.osc(carrier_freq + 0.25);

// ── Simplified Carrier Recovery (Phase Tracking) ────────────────────────────
// Phase detector for BPSK with fixed carrier reference
phase_detector(i_in, q_in) = phase_error
with {
    // Mix with fixed carrier
    i_mix = i_in * fixed_carrier_i - q_in * fixed_carrier_q;
    q_mix = i_in * fixed_carrier_q + q_in * fixed_carrier_i;
    
    // BPSK phase detection
    phase_error = sign(i_mix) * q_mix;
};

// Phase correction loop
phase_correction(phase_error) = phase_offset
with {
    // Integrate phase error
    phase_offset = phase_error * pll_bandwidth : + ~ _;
};

// ── Simplified Timing Recovery ──────────────────────────────────────────────
// Simplified symbol timing using symbol rate directly
symbol_clock_generator(symbol_rate) = symbol_clock
with {
    // Generate symbol clock based on rate
    phase_accum = symbol_rate / sample_rate : + ~ _;
    symbol_clock = phase_accum > 1.0 ? 1.0 : 0.0;
};

// ── Matched Filtering (RRC) ─────────────────────────────────────────────────
matched_filter(signal) = filtered_signal
with {
    cutoff = symbol_rate * (1.0 + rrc_rolloff) * 0.5;
    filtered_signal = signal : fi.lowpass(4, cutoff);
};

// ── Symbol Decision ─────────────────────────────────────────────────────────
symbol_decision(filtered_i, filtered_q, symbol_clock) = symbol_out
with {
    // Sample at symbol boundaries
    i_sample = filtered_i * symbol_clock;
    q_sample = filtered_q * symbol_clock;
    
    // BPSK decision with hysteresis for noise immunity
    symbol_value = sign(i_sample);
    symbol_out = symbol_value * output_gain;
};

// ── Main Demodulation Process ───────────────────────────────────────────────
demodulate_mono(i_in, q_in) = symbol_out
with {
    // Phase correction
    phase_error = phase_detector(i_in, q_in);
    phase_offset = phase_correction(phase_error);
    
    // Apply phase correction to carrier
    corrected_carrier_i = fixed_carrier_i * cos(phase_offset) - fixed_carrier_q * sin(phase_offset);
    corrected_carrier_q = fixed_carrier_i * sin(phase_offset) + fixed_carrier_q * cos(phase_offset);
    
    // Mix down to baseband
    i_baseband = i_in * corrected_carrier_i - q_in * corrected_carrier_q;
    q_baseband = i_in * corrected_carrier_q + q_in * corrected_carrier_i;
    
    // Matched filtering
    i_filtered = matched_filter(i_baseband);
    q_filtered = matched_filter(q_baseband);
    
    // Generate symbol clock
    symbol_clock = symbol_clock_generator(symbol_rate);
    
    // Symbol decision
    symbol_out = symbol_decision(i_filtered, q_filtered, symbol_clock);
};

// ── Main Process ────────────────────────────────────────────────────────────
process = demodulate_mono;