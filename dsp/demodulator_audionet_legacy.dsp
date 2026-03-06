// demodulator_audionet_legacy.dsp — AudioNet Legacy Dial-Up Demodulator
//
// Fixed-rate BPSK demodulator with authentic connection state simulation
// Pure nostalgic dial-up experience with classic sound profiles
// Designed to match the AudioNet legacy modulator
//
// 2 inputs  (I, Q from audio interface)
// 1 output (demodulated BPSK symbols ±1.0 for Haskell transport)

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Legacy sound profile selection
sound_profile = hslider("/demodulator_audionet_legacy/sound_profile", 0, 0, 3, 1);

// Connection state simulation
connection_state = hslider("/demodulator_audionet_legacy/connection_state", 0, 0, 4, 1);

// Output gain control
output_gain = hslider("/demodulator_audionet_legacy/output_gain", 1.0, 0.0, 2.0, 0.01);

// ── Legacy Sound Profiles (Fixed Rates and Frequencies) ─────────────────────
// Profile 0: 300 baud Teletype
profile_300_freq = select2(sound_profile == 0, 0, 0) : select(1800);
profile_300_rate = select2(sound_profile == 0, 0, 0) : select(300);

// Profile 1: 1200 baud Early Modem
profile_1200_freq = select2(sound_profile == 1, 0, 0) : select(2200);
profile_1200_rate = select2(sound_profile == 1, 0, 0) : select(1200);

// Profile 2: 2400 baud Classic Modem
profile_2400_freq = select2(sound_profile == 2, 0, 0) : select(2400);
profile_2400_rate = select2(sound_profile == 2, 0, 0) : select(2400);

// Profile 3: 9600 baud Fast Modem
profile_9600_freq = select2(sound_profile == 3, 0, 0) : select(2800);
profile_9600_rate = select2(sound_profile == 3, 0, 0) : select(9600);

// Combined parameters
carrier_freq = profile_300_freq + profile_1200_freq + profile_2400_freq + profile_9600_freq;
symbol_rate = profile_300_rate + profile_1200_rate + profile_2400_rate + profile_9600_rate;

// ── Connection State Simulation (Dial-up Experience) ────────────────────────
// State 0: Idle
// State 1: Dialing (detect 1.2kHz tone)
// State 2: Handshake (detect alternating beeps)
// State 3: Connected (normal BPSK demodulation)
// State 4: Disconnecting (detect descending tone)

// Tone detection for connection states
dial_tone_detector(input) = detected
with {
    // Bandpass filter around 1.2kHz
    filtered = input : fi.bandpass(4, 1100, 1300);
    envelope = abs(filtered) : si.smoo;
    detected = envelope > 0.1 ? 1.0 : 0.0;
};

handshake_detector(input) = detected
with {
    // Detect alternating beeps around 2kHz
    filtered = input : fi.bandpass(4, 1900, 2100);
    envelope = abs(filtered) : si.smoo;
    detected = envelope > 0.05 ? 1.0 : 0.0;
};

disconnect_detector(input) = detected
with {
    // Detect descending tone from 3kHz to 1kHz
    filtered = input : fi.bandpass(4, 900, 3100);
    envelope = abs(filtered) : si.smoo;
    detected = envelope > 0.1 ? 1.0 : 0.0;
};

// Connection state machine
connection_state_machine(input) = current_state
with {
    dial_detected = dial_tone_detector(input);
    handshake_detected = handshake_detector(input);
    disconnect_detected = disconnect_detector(input);
    
    // State transitions
    next_state = select2(current_state == 0 && dial_detected, 1,
                select2(current_state == 1 && handshake_detected, 2,
                select2(current_state == 2 && !handshake_detected, 3,
                select2(current_state == 3 && disconnect_detected, 4,
                current_state))));
    
    current_state = next_state : delay(1);
};

// ── Fixed Carrier Generation ────────────────────────────────────────────────
// Generate fixed carrier based on selected profile
fixed_carrier_i = os.osc(carrier_freq);
fixed_carrier_q = os.osc(carrier_freq + 0.25);

// ── Simplified Demodulation for Legacy Rates ────────────────────────────────
legacy_demodulate(i_in, q_in) = symbol_out
with {
    // Mix down to baseband
    i_baseband = i_in * fixed_carrier_i - q_in * fixed_carrier_q;
    q_baseband = i_in * fixed_carrier_q + q_in * fixed_carrier_i;
    
    // Matched filtering optimized for legacy rates
    i_filtered = i_baseband : fi.lowpass(4, symbol_rate * 0.6);
    q_filtered = q_baseband : fi.lowpass(4, symbol_rate * 0.6);
    
    // Simple symbol timing (fixed rate)
    symbol_clock = (symbol_rate / sample_rate) > 0.5 ? 1.0 : 0.0;
    
    // Symbol decision
    i_sample = i_filtered * symbol_clock;
    symbol_out = sign(i_sample) * output_gain;
};

// ── Connection Sound Generation ─────────────────────────────────────────────
// Generate authentic dial-up sounds for connection states
connection_sounds(state) = sound_output
with {
    // Dial tone (1.2kHz)
    dial_tone = os.osc(1200) * (state == 1 ? 1.0 : 0.0);
    
    // Handshake beeps (alternating 2.2kHz and 1.8kHz)
    handshake_beep1 = os.osc(2200) * (state == 2 ? 1.0 : 0.0);
    handshake_beep2 = os.osc(1800) * (state == 2 ? 1.0 : 0.0);
    
    // Disconnect tone (descending from 3kHz to 1kHz)
    disconnect_phase = (state == 4 ? 1.0 : 0.0) : + ~ _;
    disconnect_freq = 3000.0 - (disconnect_phase * 2000.0);
    disconnect_tone = os.osc(disconnect_freq) * (state == 4 ? 1.0 : 0.0);
    
    sound_output = (dial_tone + handshake_beep1 + handshake_beep2 + disconnect_tone) * output_gain;
};

// ── Main Demodulation Process ───────────────────────────────────────────────
demodulate_legacy(i_in, q_in) = symbol_out
with {
    // Detect connection state
    detected_state = connection_state_machine(i_in);
    
    // Generate connection sounds
    sounds = connection_sounds(detected_state);
    
    // Legacy demodulation
    demod_symbols = legacy_demodulate(i_in, q_in);
    
    // Select output based on connection state
    symbol_out = select2(detected_state == 3, demod_symbols, sounds);
};

// ── Main Process ────────────────────────────────────────────────────────────
process = demodulate_legacy;