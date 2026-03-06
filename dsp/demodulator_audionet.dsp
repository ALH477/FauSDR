// demodulator_audionet.dsp — AudioNet Variable-Rate BPSK Demodulator
//
// Coherent BPSK demodulator with adaptive carrier recovery and timing recovery
// Supports variable symbol rates (1200-38400 baud) with audible carrier frequencies
// Designed to match the AudioNet modulator family
//
// 2 inputs  (I, Q from audio interface or SoapySDR)
// 1 output (demodulated BPSK symbols ±1.0 for Haskell transport)

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Sample rate selection for different audio quality levels
sample_rate = hslider("/demodulator_audionet/sample_rate[unit:Hz]", 48000, 44100, 192000, 1000) : si.smoo;

// Expected carrier frequency for coherent detection (1-4kHz range)
expected_carrier = hslider("/demodulator_audionet/expected_carrier[unit:Hz]", 2200, 1000, 4000, 10) : si.smoo;

// Symbol rate for timing recovery (1200-38400 baud)
expected_symbol_rate = hslider("/demodulator_audionet/expected_symbol_rate[unit:Sym/s]", 9600, 1200, 38400, 100) : si.smoo;

// PLL parameters for carrier recovery
pll_bandwidth = hslider("/demodulator_audionet/pll_bandwidth", 0.01, 0.001, 0.1, 0.001) : si.smoo;

// Timing recovery parameters
timing_gain = hslider("/demodulator_audionet/timing_gain", 0.1, 0.01, 1.0, 0.01) : si.smoo;

// RRC filter parameters
rrc_rolloff = hslider("/demodulator_audionet/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;

// Detection threshold
detection_threshold = hslider("/demodulator_audionet/detection_threshold", 0.1, 0.01, 1.0, 0.01) : si.smoo;

// Output gain
output_gain = hslider("/demodulator_audionet/output_gain", 1.0, 0.0, 2.0, 0.01);

// ── Preset Buttons for Common Rates ─────────────────────────────────────────
// Rate presets
rate_1200 = button("/demodulator_audionet/presets/1200_baud") : select(1200);
rate_2400 = button("/demodulator_audionet/presets/2400_baud") : select(2400);
rate_9600 = button("/demodulator_audionet/presets/9600_baud") : select(9600);
rate_19200 = button("/demodulator_audionet/presets/19200_baud") : select(19200);
rate_38400 = button("/demodulator_audionet/presets/38400_baud") : select(38400);

// ── Carrier Recovery (PLL-based) ────────────────────────────────────────────
// Phase detector for BPSK
phase_detector(i_in, q_in, carrier_i, carrier_q) = phase_error
with {
    // Multiply received signal with carrier
    i_mix = i_in * carrier_i - q_in * carrier_q;
    q_mix = i_in * carrier_q + q_in * carrier_i;
    
    // BPSK phase detection (sign of I component)
    phase_error = sign(i_mix) * q_mix;
};

// VCO for carrier generation
carrier_vco(freq, phase_offset) = carrier_i, carrier_q
with {
    phase = phase_offset + freq / sample_rate;
    carrier_i = cos(2 * ma.PI * phase);
    carrier_q = sin(2 * ma.PI * phase);
};

// PLL implementation
pll_loop(i_in, q_in, freq_estimate) = carrier_i, carrier_q, freq_out
with {
    (carrier_i, carrier_q) = carrier_vco(freq_estimate, 0.0);
    phase_error = phase_detector(i_in, q_in, carrier_i, carrier_q);
    
    // Loop filter (proportional)
    freq_correction = phase_error * pll_bandwidth;
    freq_out = freq_estimate + freq_correction;
};

// ── Symbol Timing Recovery (Gardner Algorithm) ──────────────────────────────
// Gardner timing error detector
gardner_detector(i_in, q_in, symbol_rate) = timing_error
with {
    // Delay elements for early/late sampling
    i_early = delay(i_in, 0.25 / symbol_rate);
    i_late = delay(i_in, 0.75 / symbol_rate);
    q_early = delay(q_in, 0.25 / symbol_rate);
    q_late = delay(q_in, 0.75 / symbol_rate);
    
    // Gardner timing error
    timing_error = (i_in - i_late) * i_early + (q_in - q_late) * q_early;
};

// Timing recovery loop
timing_loop(i_in, q_in, symbol_rate_estimate) = symbol_clock, symbol_rate_out
with {
    timing_error = gardner_detector(i_in, q_in, symbol_rate_estimate);
    
    // Loop filter for timing
    timing_correction = timing_error * timing_gain;
    symbol_rate_out = symbol_rate_estimate + timing_correction;
    
    // Generate symbol clock
    symbol_clock = (symbol_rate_out / sample_rate) > 0.5 ? 1.0 : 0.0;
};

// ── Matched Filtering (RRC) ─────────────────────────────────────────────────
matched_filter(signal, symbol_rate) = filtered_signal
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
    
    // BPSK decision (I component determines symbol)
    symbol_out = sign(i_sample) * output_gain;
};

// ── Main Demodulation Process ───────────────────────────────────────────────
demodulate(i_in, q_in) = symbol_out
with {
    // Carrier recovery
    (carrier_i, carrier_q, freq_out) = pll_loop(i_in, q_in, expected_carrier);
    
    // Mix down to baseband
    i_baseband = i_in * carrier_i - q_in * carrier_q;
    q_baseband = i_in * carrier_q + q_in * carrier_i;
    
    // Matched filtering
    i_filtered = matched_filter(i_baseband, expected_symbol_rate);
    q_filtered = matched_filter(q_baseband, expected_symbol_rate);
    
    // Timing recovery
    (symbol_clock, symbol_rate_out) = timing_loop(i_filtered, q_filtered, expected_symbol_rate);
    
    // Symbol decision
    symbol_out = symbol_decision(i_filtered, q_filtered, symbol_clock);
};

// ── Main Process ────────────────────────────────────────────────────────────
process = demodulate;