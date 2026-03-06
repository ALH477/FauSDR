// modulator_audionet_legacy.dsp — AudioNet Legacy Dial-Up Style Modulator
//
// Pure nostalgic dial-up experience with classic sound profiles
// Fixed rates and frequencies for authentic modem sounds
//
// 1 input  (BPSK symbol stream ±1.0 from Haskell)
// 2 outputs (I, Q) for audio interface

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── UI Parameters (set via DCF.Faust.DSP.setParam) ──────────────────────────
// Legacy sound profile selection
sound_profile = hslider("/audionet_legacy/sound_profile", 0, 0, 3, 1);

// Connection state simulation
connection_state = hslider("/audionet_legacy/connection_state", 0, 0, 4, 1);

// Output gain control
output_gain = hslider("/audionet_legacy/output_gain", 0.8, 0.0, 1.0, 0.001);

// ── Legacy Sound Profiles (Classic Modem Sounds) ────────────────────────────
// Profile 0: 300 baud Teletype
profile_300 = select2(sound_profile == 0, 0, 0) : select(1800);
rate_300 = select2(sound_profile == 0, 0, 0) : select(300);

// Profile 1: 1200 baud Early Modem
profile_1200 = select2(sound_profile == 1, 0, 0) : select(2200);
rate_1200 = select2(sound_profile == 1, 0, 0) : select(1200);

// Profile 2: 2400 baud Classic Modem
profile_2400 = select2(sound_profile == 2, 0, 0) : select(2400);
rate_2400 = select2(sound_profile == 2, 0, 0) : select(2400);

// Profile 3: 9600 baud Fast Modem
profile_9600 = select2(sound_profile == 3, 0, 0) : select(2800);
rate_9600 = select2(sound_profile == 3, 0, 0) : select(9600);

// Combined parameters
carrier_freq = profile_300 + profile_1200 + profile_2400 + profile_9600;
symbol_rate = rate_300 + rate_1200 + rate_2400 + rate_9600;

// ── Connection State Simulation (Dial-up Experience) ────────────────────────
// State 0: Idle
// State 1: Dialing (1.2kHz rising tone)
// State 2: Handshake (alternating beeps)
// State 3: Connected (data transmission)
// State 4: Disconnecting (descending tone)

// Dial tone generator
dial_tone = os.osc(1200) * (connection_state == 1 ? 1 : 0);

// Handshake beeps
handshake_beep1 = os.osc(2200) * (connection_state == 2 ? 1 : 0);
handshake_beep2 = os.osc(1800) * (connection_state == 2 ? 1 : 0);

// Data transmission (normal BPSK)
data_transmission = connection_state == 3 ? 1 : 0;

// Disconnect tone
disconnect_tone = os.osc(3000) * (connection_state == 4 ? 1 : 0);

// ── RRC-approximating pulse shaping for legacy rates ────────────────────────
pulse_shape(sym) = sym : fi.lowpass(4, symbol_rate * 0.6);

// ── Quadrature NCO carrier ──────────────────────────────────────────────────
carrier_i = os.osc(carrier_freq);          // cosine
carrier_q = os.osc(carrier_freq + 0.25);   // sine (+90°)

// ── Phase modulation ────────────────────────────────────────────────────────
pm_modulate(sym) =
    (i_mod * output_gain, q_mod * output_gain)
with {
    phi   = pulse_shape(sym) * 3.14159;
    i_mod = carrier_i * cos(phi) - carrier_q * sin(phi);
    q_mod = carrier_i * sin(phi) + carrier_q * cos(phi);
};

// ── Connection sound mixing ─────────────────────────────────────────────────
connection_sounds = (
    (dial_tone + handshake_beep1 + handshake_beep2 + disconnect_tone) * output_gain,
    (dial_tone + handshake_beep1 + handshake_beep2 + disconnect_tone) * output_gain
);

// ── Main process with connection state selection ────────────────────────────
process = select2(data_transmission, connection_sounds, pm_modulate);