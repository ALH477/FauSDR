// demodulator_audionet_stereo.dsp — AudioNet Stereo Dual-Channel Demodulator
//
// Dual-channel coherent BPSK demodulator for stereo audio hardware
// Independent demodulation of left and right channels with diversity combining
// Higher throughput with parallel processing and channel error detection
// Designed to match the AudioNet stereo modulator
//
// 4 inputs  (I_left, Q_left, I_right, Q_right from stereo audio interface)
// 2 outputs (demodulated BPSK symbols ±1.0 for left and right channels)

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Sample rate selection for different audio quality levels
sample_rate = hslider("/demodulator_audionet_stereo/sample_rate[unit:Hz]", 96000, 44100, 192000, 1000) : si.smoo;

// Carrier frequencies for each channel (1-4kHz range)
carrier_freq_left = hslider("/demodulator_audionet_stereo/carrier_freq_left[unit:Hz]", 2200, 1000, 4000, 10) : si.smoo;
carrier_freq_right = hslider("/demodulator_audionet_stereo/carrier_freq_right[unit:Hz]", 2600, 1000, 4000, 10) : si.smoo;

// Symbol rates for each channel (1200-38400 baud)
symbol_rate_left = hslider("/demodulator_audionet_stereo/symbol_rate_left[unit:Sym/s]", 9600, 1200, 38400, 100) : si.smoo;
symbol_rate_right = hslider("/demodulator_audionet_stereo/symbol_rate_right[unit:Sym/s]", 4800, 1200, 38400, 100) : si.smoo;

// PLL parameters for carrier recovery
pll_bandwidth = hslider("/demodulator_audionet_stereo/pll_bandwidth", 0.01, 0.001, 0.1, 0.001) : si.smoo;

// Timing recovery parameters
timing_gain = hslider("/demodulator_audionet_stereo/timing_gain", 0.1, 0.01, 1.0, 0.01) : si.smoo;

// RRC filter parameters
rrc_rolloff = hslider("/demodulator_audionet_stereo/rrc_rolloff", 0.35, 0.1, 0.9, 0.01) : si.smoo;

// Diversity combining mode
diversity_mode = hslider("/demodulator_audionet_stereo/diversity_mode", 0, 0, 2, 1);

// Output gains
output_gain_left = hslider("/demodulator_audionet_stereo/output_gain_left", 1.0, 0.0, 2.0, 0.01);
output_gain_right = hslider("/demodulator_audionet_stereo/output_gain_right", 1.0, 0.0, 2.0, 0.01);

// ── Preset Buttons for Common Configurations ────────────────────────────────
// Rate presets
rate_1200 = button("/demodulator_audionet_stereo/presets/1200_baud") : select(1200);
rate_2400 = button("/demodulator_audionet_stereo/presets/2400_baud") : select(2400);
rate_9600 = button("/demodulator_audionet_stereo/presets/9600_baud") : select(9600);
rate_19200 = button("/demodulator_audionet_stereo/presets/19200_baud") : select(19200);
rate_38400 = button("/demodulator_audionet_stereo/presets/38400_baud") : select(38400);

// ── Carrier Generation for Both Channels ───────────────────────────────────
// Generate carriers for left and right channels
carrier_left_i = os.osc(carrier_freq_left);
carrier_left_q = os.osc(carrier_freq_left + 0.25);
carrier_right_i = os.osc(carrier_freq_right);
carrier_right_q = os.osc(carrier_freq_right + 0.25);

// ── Single Channel Demodulator Module ───────────────────────────────────────
single_channel_demod(i_in, q_in, carrier_i_ref, carrier_q_ref, symbol_rate_ch, output_gain_ch) = symbol_out
with {
    // Phase detector for BPSK
    phase_detector(i_in, q_in, carrier_i, carrier_q) = phase_error
    with {
        i_mix = i_in * carrier_i - q_in * carrier_q;
        q_mix = i_in * carrier_q + q_in * carrier_i;
        phase_error = sign(i_mix) * q_mix;
    };
    
    // PLL implementation
    pll_loop(i_in, q_in, freq_estimate) = carrier_i, carrier_q, freq_out
    with {
        phase = freq_estimate / sample_rate : + ~ _;
        carrier_i = cos(2 * ma.PI * phase);
        carrier_q = sin(2 * ma.PI * phase);
        phase_error = phase_detector(i_in, q_in, carrier_i, carrier_q);
        freq_correction = phase_error * pll_bandwidth;
        freq_out = freq_estimate + freq_correction;
    };
    
    // Gardner timing error detector
    gardner_detector(i_in, q_in, symbol_rate) = timing_error
    with {
        i_early = delay(i_in, 0.25 / symbol_rate);
        i_late = delay(i_in, 0.75 / symbol_rate);
        q_early = delay(q_in, 0.25 / symbol_rate);
        q_late = delay(q_in, 0.75 / symbol_rate);
        timing_error = (i_in - i_late) * i_early + (q_in - q_late) * q_early;
    };
    
    // Timing recovery loop
    timing_loop(i_in, q_in, symbol_rate_estimate) = symbol_clock, symbol_rate_out
    with {
        timing_error = gardner_detector(i_in, q_in, symbol_rate_estimate);
        timing_correction = timing_error * timing_gain;
        symbol_rate_out = symbol_rate_estimate + timing_correction;
        symbol_clock = (symbol_rate_out / sample_rate) > 0.5 ? 1.0 : 0.0;
    };
    
    // Matched filtering
    matched_filter(signal, symbol_rate) = filtered_signal
    with {
        cutoff = symbol_rate * (1.0 + rrc_rolloff) * 0.5;
        filtered_signal = signal : fi.lowpass(4, cutoff);
    };
    
    // Symbol decision
    symbol_decision(filtered_i, filtered_q, symbol_clock) = symbol_out
    with {
        i_sample = filtered_i * symbol_clock;
        q_sample = filtered_q * symbol_clock;
        symbol_out = sign(i_sample) * output_gain_ch;
    };
    
    // Main demodulation process for single channel
    (carrier_i, carrier_q, freq_out) = pll_loop(i_in, q_in, carrier_i_ref);
    i_baseband = i_in * carrier_i - q_in * carrier_q;
    q_baseband = i_in * carrier_q + q_in * carrier_i;
    i_filtered = matched_filter(i_baseband, symbol_rate_ch);
    q_filtered = matched_filter(q_baseband, symbol_rate_ch);
    (symbol_clock, symbol_rate_out) = timing_loop(i_filtered, q_filtered, symbol_rate_ch);
    symbol_out = symbol_decision(i_filtered, q_filtered, symbol_clock);
};

// ── Channel Quality Monitoring ──────────────────────────────────────────────
channel_quality(i_in, q_in) = snr_estimate
with {
    // Simple SNR estimation based on signal power vs noise floor
    signal_power = (i_in * i_in + q_in * q_in) : si.smoo;
    noise_floor = 0.001; // Estimated noise floor
    snr_estimate = max(0.0, signal_power / noise_floor);
};

// ── Diversity Combining Logic ───────────────────────────────────────────────
diversity_combiner(symbol_left, symbol_right, snr_left, snr_right, mode) = combined_symbol
with {
    // Mode 0: Selection combining (best channel)
    // Mode 1: Equal gain combining
    // Mode 2: Maximal ratio combining
    
    selection_weight = select2(mode == 0, 0.0, 1.0);
    equal_weight = select2(mode == 1, 0.5, 1.0);
    ratio_weight_left = select2(mode == 2, snr_left / (snr_left + snr_right), 0.5);
    ratio_weight_right = select2(mode == 2, snr_right / (snr_left + snr_right), 0.5);
    
    combined_symbol = symbol_left * ratio_weight_left + symbol_right * ratio_weight_right;
};

// ── Main Stereo Demodulation Process ────────────────────────────────────────
demodulate_stereo(i_left, q_left, i_right, q_right) = symbol_left_out, symbol_right_out
with {
    // Demodulate left channel
    symbol_left = single_channel_demod(i_left, q_left, carrier_left_i, carrier_left_q, symbol_rate_left, output_gain_left);
    
    // Demodulate right channel
    symbol_right = single_channel_demod(i_right, q_right, carrier_right_i, carrier_right_q, symbol_rate_right, output_gain_right);
    
    // Monitor channel quality
    snr_left = channel_quality(i_left, q_left);
    snr_right = channel_quality(i_right, q_right);
    
    // Apply diversity combining if enabled
    combined_symbol = diversity_combiner(symbol_left, symbol_right, snr_left, snr_right, diversity_mode);
    
    // Output selection
    symbol_left_out = diversity_mode == 0 ? symbol_left : combined_symbol;
    symbol_right_out = diversity_mode == 0 ? symbol_right : combined_symbol;
};

// ── Main Process ────────────────────────────────────────────────────────────
process = demodulate_stereo;