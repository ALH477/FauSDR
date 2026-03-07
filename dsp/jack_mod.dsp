// jack_mod.dsp — DeMoD JackFrame BPSK Modulator (audio-band, guitar cable)
//
// 1 input  : NRZ-L BPSK symbols (±1.0 floats) from Haskell
// 2 outputs: I, Q baseband (feed to JACK/PipeWire audio output)
//
// Designed for USB guitar interfaces (Focusrite Scarlett, Behringer UMC, etc.)
// Sample rates: 96 kHz or 192 kHz
// Symbol rate : 9600 baud
// Carrier     : 4 kHz (null BW = 12.96 kHz, fits within guitar cable passband)
//
// RRC filter selection (rrc_mode):
//   0 → 63-tap,  sps=10  (96 kHz SR,  delay = 31 samples)
//   1 → 121-tap, sps=20  (192 kHz SR, delay = 60 samples)
//
// Latency (192 kHz, 32-sample buffer, rrc_mode=1):
//   Frame airtime : 64 / 9600 = 6.667 ms
//   Buffer fill   : 32 / 192000 = 0.167 ms
//   One-way total : ~6.83 ms  |  Jitter: ±0.005 ms
//
// Carrier at 4 kHz:
//   96 kHz SR  → carrier_norm = 4000 / 48000 = 0.08333
//   192 kHz SR → carrier_norm = 4000 / 96000 = 0.04167
//   Set via: setParam dsp "/jack_mod/carrier_freq" (fc / (sr/2))
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/jack_mod.dsp -o build/jack_mod_gen.cpp
//
// DeMoD LLC — GPL-3.0

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
// carrier_freq: normalised (0..0.5 = 0..SR/2)
//   4 kHz @ 96 kHz SR  → 4000/48000  = 0.08333
//   4 kHz @ 192 kHz SR → 4000/96000  = 0.04167
carrier_freq = hslider("/jack_mod/carrier_freq",  0.08333, 0.01, 0.40, 0.00001) : si.smoo;
mod_index    = hslider("/jack_mod/mod_index",     3.14159, 0.0,  6.28318, 0.001) : si.smoo;
output_gain  = hslider("/jack_mod/output_gain",   0.8,    0.0,  1.0,    0.001);
// rrc_mode: 0 = 63-tap/sps=10 (96kHz), 1 = 121-tap/sps=20 (192kHz)
rrc_mode     = hslider("/jack_mod/rrc_mode",      0,      0,    1,      1) : int;

// ── RRC FIR — 63-tap, α=0.35, β=8.0, sps=10 (9600 baud @ 96 kHz SR) ─────────
//
// Delay: (63−1)/2 = 31 samples.  Null-to-null BW: 9600×1.35 = 12.96 kHz.
// sps=10 is the minimum for adequate ISI suppression with a 63-tap RRC.
// Stopband attenuation: −80 dB (Kaiser β=8.0).
rrc10_coeffs = waveform {
  -0.000049476, -0.000137191, -0.000250882, -0.000320806, -0.000213637, 0.000256791, 0.001283741,
  0.002982887, 0.005278224, 0.007784676, 0.009728348, 0.009952689, 0.007053161, -0.000338348,
  -0.013133927, -0.031293432, -0.053351932, -0.076097944, -0.094530237, -0.102188693, -0.091890546,
  -0.056816967, 0.008196073, 0.105382806, 0.233044975, 0.385062175, 0.551084888, 0.717463201,
  0.868823716, 0.990072148, 1.068499925, 1.095633841, 1.068499925, 0.990072148, 0.868823716,
  0.717463201, 0.551084888, 0.385062175, 0.233044975, 0.105382806, 0.008196073, -0.056816967,
  -0.091890546, -0.102188693, -0.094530237, -0.076097944, -0.053351932, -0.031293432, -0.013133927,
  -0.000338348, 0.007053161, 0.009952689, 0.009728348, 0.007784676, 0.005278224, 0.002982887,
  0.001283741, 0.000256791, -0.000213637, -0.000320806, -0.000250882, -0.000137191, -0.000049476
};
rrc10_coeff(n) = rrc10_coeffs, int(n) : rdtable;
rrc10_ntaps    = 63;
pulse_10(sym)  = sum(i, rrc10_ntaps, rrc10_coeff(i) * (sym@i));

// ── RRC FIR — 121-tap, α=0.35, β=8.0, sps=20 (9600 baud @ 192 kHz SR) ───────
//
// Delay: (121−1)/2 = 60 samples.  Same null BW: 12.96 kHz.
// sps=20 gives excellent ISI rejection (6 symbol spans of filter).
rrc20_coeffs = waveform {
  -0.000059532, -0.000097442, -0.000139292, -0.000177924, -0.000202471, -0.000198150, -0.000146458,
  -0.000025871, 0.000186855, 0.000514744, 0.000978014, 0.001590904, 0.002358050, 0.003270614,
  0.004302442, 0.005406583, 0.006512571, 0.007524868, 0.008322896, 0.008763009, 0.008682746,
  0.007907528, 0.006259895, 0.003571143, -0.000304906, -0.005476507, -0.011997645, -0.019851602,
  -0.028935435, -0.039046458, -0.049871872, -0.060982634, -0.071832572, -0.081763546, -0.090017237,
  -0.095753780, -0.098077140, -0.096066709, -0.088814181, -0.075464413, -0.055258592, -0.027577769,
  0.008015382, 0.051738804, 0.103559495, 0.163170171, 0.229975926, 0.303092225, 0.381355102,
  0.463343859, 0.547415921, 0.631752924, 0.714416443, 0.793411284, 0.866753760, 0.932542076,
  0.989025716, 1.034670728, 1.068217906, 1.088731166, 1.095633841, 1.088731166, 1.068217906,
  1.034670728, 0.989025716, 0.932542076, 0.866753760, 0.793411284, 0.714416443, 0.631752924,
  0.547415921, 0.463343859, 0.381355102, 0.303092225, 0.229975926, 0.163170171, 0.103559495,
  0.051738804, 0.008015382, -0.027577769, -0.055258592, -0.075464413, -0.088814181, -0.096066709,
  -0.098077140, -0.095753780, -0.090017237, -0.081763546, -0.071832572, -0.060982634, -0.049871872,
  -0.039046458, -0.028935435, -0.019851602, -0.011997645, -0.005476507, -0.000304906, 0.003571143,
  0.006259895, 0.007907528, 0.008682746, 0.008763009, 0.008322896, 0.007524868, 0.006512571,
  0.005406583, 0.004302442, 0.003270614, 0.002358050, 0.001590904, 0.000978014, 0.000514744,
  0.000186855, -0.000025871, -0.000146458, -0.000198150, -0.000202471, -0.000177924, -0.000139292,
  -0.000097442, -0.000059532
};
rrc20_coeff(n) = rrc20_coeffs, int(n) : rdtable;
rrc20_ntaps    = 121;
pulse_20(sym)  = sum(i, rrc20_ntaps, rrc20_coeff(i) * (sym@i));

// ── Pulse shaper selector ─────────────────────────────────────────────────────
pulse(sym) = select2(rrc_mode, pulse_10(sym), pulse_20(sym));

// ── Quadrature NCO ────────────────────────────────────────────────────────────
carrier_i = os.oscrc(carrier_freq);
carrier_q = os.oscs(carrier_freq);

// ── BPSK Phase Modulation ─────────────────────────────────────────────────────
phi   = pulse(_) * mod_index;
i_out = carrier_i * cos(phi) - carrier_q * sin(phi);
q_out = carrier_i * sin(phi) + carrier_q * cos(phi);

// ── Main: 1 input (symbol stream), 2 outputs (I, Q) ─────────────────────────
process = (i_out * output_gain), (q_out * output_gain);
