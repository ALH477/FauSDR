// acoustic_fsk_demod.dsp — DeMoD Acoustic FSK Demodulator
//
// Receives two-tone FSK audio from a microphone and outputs soft bit decisions.
// Works with laptop built-in mics, USB mics, phone mics — anything with
// a flat-ish response from 1–4 kHz.
//
// Architecture:
//   Mono mic input → bandpass prefilter → dual-filter energy detector
//                 → symbol integrator → soft bit output
//
// The dual-filter energy detector is the correct algorithm for two-tone FSK:
//
//   1. Bandpass prefilter (1200–3800 Hz) — rejects hum, hiss, speech above 4kHz
//   2. Two resonant bandpass filters:
//        BPF_mark  centred at f_mark  (2000 Hz), Q = 3.3
//        BPF_space centred at f_space (3000 Hz), Q = 5.0
//   3. Envelope follower on each: E_mark = lpf(BPF_mark(x)²)
//                                 E_space = lpf(BPF_space(x)²)
//   4. Decision: soft_bit = E_mark - E_space
//        positive → mark (bit 1)
//        negative → space (bit 0)
//   5. Symbol integrator (lowpass at Rs/2): averages over one symbol period
//        to maximise SNR before the Haskell slicer samples it
//
// Why dual-filter beats FM discriminator for this application:
//   - No Hilbert transform needed (mic gives real signal, not IQ)
//   - Robust to multipath: reflections cause amplitude modulation, not
//     phase corruption — energy detector is amplitude-insensitive at decision
//   - Robust to frequency drift: BPF pass-bands are ~600 Hz wide,
//     so ±100 Hz clock drift is absorbed without threshold shifting
//   - Noise in one filter doesn't corrupt the other — differential decision
//     cancels common-mode noise (ambient hum, HVAC, etc.)
//
// AGC note:
//   The demodulator is DC-blocked and amplitude-normalised before the BPFs.
//   Mic preamp gain does not affect the bit decisions — only SNR matters.
//
// Parameters:
//   /acoustic_fsk_demod/f_mark      — mark freq Hz (must match TX, default 2000)
//   /acoustic_fsk_demod/f_space     — space freq Hz (must match TX, default 3000)
//   /acoustic_fsk_demod/symbol_rate — baud rate (must match TX, default 1200)
//   /acoustic_fsk_demod/bpf_q       — BPF Q factor (default 3.5, higher=narrower)
//   /acoustic_fsk_demod/output_gain — soft symbol scale (default 4.0)
//
// Inputs:  1 — mono microphone audio (raw, any gain)
// Outputs: 1 — soft bit decisions (+ve = mark/1, -ve = space/0)
//              Haskell samples this at symbol centres (every sps=40 samples)
//              and thresholds at 0.
//
// Build:
//   faust -a alsa-gtk.cpp -lang c++ -vec -vs 256 \
//         dsp/acoustic_fsk_demod.dsp -o build/acoustic_fsk_demod_gen.cpp
//
// DeMoD LLC — GPL-3.0

import("stdfaust.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
f_mark      = hslider("/acoustic_fsk_demod/f_mark",      2000, 200,  8000, 1)    : si.smoo;
f_space     = hslider("/acoustic_fsk_demod/f_space",     3000, 200,  8000, 1)    : si.smoo;
symbol_rate = hslider("/acoustic_fsk_demod/symbol_rate", 1200, 300,  4800, 1)    : si.smoo;
// Q factor for mark/space BPFs.
// Q = fc / BW_3dB. Higher Q → narrower filter → better selectivity but
// slower step response. Q=3.5 gives BW ≈ 570–860 Hz, settling in ~sps/2 samples.
bpf_q       = hslider("/acoustic_fsk_demod/bpf_q",       3.5,  1.0, 20.0, 0.1);
output_gain = hslider("/acoustic_fsk_demod/output_gain",  4.0,  0.0, 20.0, 0.1);

// ── Stage 1: DC block + bandpass prefilter ────────────────────────────────────
// Remove DC offset (mic bias) and frequencies outside the signal band.
// Prefilter band: ~800 Hz – 4000 Hz.
// This also attenuates 50/60 Hz hum and keyboard/fan noise.
dc_block(x)   = x - x';                          // 1st-order DC blocker
prefilter(x)  = x : dc_block
                  : fi.highpass(2, 800.0)         // remove low-freq hum
                  : fi.lowpass(2,  4000.0);        // remove hiss above signal

// ── Stage 2: AGC (soft normaliser) ───────────────────────────────────────────
// Normalise amplitude so BPF energy comparisons are mic-gain-independent.
// Uses a leaky peak follower: peak decays slowly, normalises signal.
// Time constant ≈ 50 ms (suitable for 1200 baud — one frame = 53 ms).
agc_tc     = 0.05;                                // 50 ms time constant
agc_coeff  = exp(-1.0 / (agc_tc * ma.SR));
peak_env(x) = (abs(x), _) ~ (\(s, p) -> max(s, p * agc_coeff));
agc(x)     = x / (peak_env(x) + 1e-6);

// ── Stage 3: Dual bandpass filters ───────────────────────────────────────────
//
// fi.resonbp(fc, Q, gain) — 2nd-order resonant bandpass filter
//   fc   = centre frequency in Hz
//   Q    = quality factor (dimensionless)
//   gain = passband gain (use 1.0)
//
// BW_3dB = fc / Q:
//   BPF_mark  at 2000 Hz, Q=3.5 → BW = 571 Hz  (passes 1714–2286 Hz)
//   BPF_space at 3000 Hz, Q=3.5 → BW = 857 Hz  (passes 2571–3429 Hz)
// Small overlap at the midpoint (~2500 Hz) — this is intentional and gives
// a smooth soft-decision at the transition between mark and space.

bpf_mark(x)  = x : fi.resonbp(f_mark,  bpf_q, 1.0);
bpf_space(x) = x : fi.resonbp(f_space, bpf_q, 1.0);

// ── Stage 4: Envelope followers (energy detectors) ───────────────────────────
//
// E(x) = lowpass(x²) — instantaneous power, smoothed.
// Time constant of the envelope follower sets the symbol integration window.
// Cutoff at symbol_rate / 2 → integrates over one symbol period.
//
// fi.lowpass(N, fc): N-th order Butterworth lowpass.
// 2nd order gives a good balance of settling speed vs noise rejection.

env_lp(x) = x * x : fi.lowpass(2, symbol_rate * 0.5);

e_mark(x)  = bpf_mark(x)  : env_lp;
e_space(x) = bpf_space(x) : env_lp;

// ── Stage 5: Differential decision ───────────────────────────────────────────
// soft_bit = E_mark - E_space
//   > 0 → mark tone dominant → bit = 1
//   < 0 → space tone dominant → bit = 0
//
// This differential form rejects common-mode amplitude changes (room reverb,
// mic distance variation) because both filters see the same ambient noise.

soft_bit(x) = e_mark(x) - e_space(x);

// ── Full chain ────────────────────────────────────────────────────────────────
acoustic_fsk_demod(mic_in) = mic_in
                            : prefilter
                            : agc
                            : soft_bit
                            : (_ * output_gain);

// ── Main: 1 input (mic), 1 output (soft bit) ─────────────────────────────────
process = acoustic_fsk_demod;
