// jack_demod.dsp — DeMoD JackFrame BPSK Demodulator (audio-band, guitar cable)
//
// 2 inputs : I, Q (from JACK/PipeWire audio input, interleaved stereo)
// 1 output : soft BPSK symbol decisions (+1.0 ≈ bit 1, −1.0 ≈ bit 0)
//
// Architecture:
//   Costas loop (carrier + phase recovery) → RRC matched filter → soft output
//
// The Costas loop at sps=10 (96 kHz / 9600 baud) has 10 samples per symbol
// to average the phase error — tighter than the 208-sps 9600-baud SDR case.
// loop_bw is set conservatively at 0.03 to avoid noise-driven frequency wander.
// Increase to 0.05 if the guitar interface has significant clock drift.
//
// USB audio clock accuracy: ±50–200 ppm typical (no TCXO).
// At 4 kHz carrier: 200 ppm = ±0.8 Hz offset → Costas pull-in range must
// cover ≥ ±0.8 Hz. With loop_bw=0.03 and sps=10: pull-in ≈ ±0.5×Rs×BL
// ≈ ±144 Hz. More than sufficient.
//
// RRC matched filter (same coefficients as jack_mod.dsp — zero ISI):
//   rrc_mode=0 → 63-tap,  sps=10  (96 kHz SR)
//   rrc_mode=1 → 121-tap, sps=20  (192 kHz SR)
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/jack_demod.dsp -o build/jack_demod_gen.cpp
//
// DeMoD LLC — GPL-3.0

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
loop_bw     = hslider("/jack_demod/loop_bw",     0.03,  0.001, 0.10, 0.001);
output_gain = hslider("/jack_demod/output_gain",  1.0,   0.0,   4.0,  0.01);
rrc_mode    = hslider("/jack_demod/rrc_mode",     0,     0,     1,    1) : int;

// ── Costas loop ───────────────────────────────────────────────────────────────
// Second-order PI loop. ζ=0.707 (critically damped).
// Kp = 4·ζ·BL,  Ki = 4·BL²
zeta = 0.707;
kp   = 4.0 * zeta * loop_bw;
ki   = 4.0 * loop_bw * loop_bw;

costas_loop(i_in, q_in) =
  letrec {
    'vco_phase  = (vco_phase + vco_freq) : ma.modulo(2.0 * ma.PI);
    'vco_i      = cos(vco_phase);
    'vco_q      = sin(vco_phase);
    'bb_i       = i_in * vco_i + q_in * vco_q;
    'bb_q       = i_in * vco_q - q_in * vco_i;
    'phase_err  = bb_i * bb_q;
    'freq_int   = freq_int + ki * phase_err;
    'vco_freq   = kp * phase_err + freq_int;
  }
  in bb_i;

// ── RRC Matched Filter — 63-tap, sps=10 (96 kHz SR) ─────────────────────────
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
mf_10(sym)     = sum(i, 63,  rrc10_coeff(i) * (sym@i));

// ── RRC Matched Filter — 121-tap, sps=20 (192 kHz SR) ────────────────────────
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
mf_20(sym)     = sum(i, 121, rrc20_coeff(i) * (sym@i));

// ── Filter selector ───────────────────────────────────────────────────────────
matched_filter(sym) = select2(rrc_mode, mf_10(sym), mf_20(sym));

// ── Full demodulator chain ────────────────────────────────────────────────────
jack_demod(i_in, q_in) = costas_loop(i_in, q_in)
                        : matched_filter
                        : (_ * output_gain);

// ── Main: 2 inputs (I, Q) → 1 output (soft symbol) ──────────────────────────
process = jack_demod;
