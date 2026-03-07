// bpsk_demod.dsp — DeMoD BPSK Demodulator
//
// BPSK coherent demodulation via Costas loop carrier recovery.
//
// Pipeline:
//   IQ input → Costas loop (carrier recovery) → matched filter (integrate-dump)
//            → soft symbol output (+1.0 / -1.0)
//
// A Costas loop is a phase-locked loop (PLL) that tracks the suppressed carrier
// of a BPSK/QPSK signal. It multiplies the incoming signal by a local quadrature
// oscillator, computes a phase error from I*Q, and feeds that error back to
// adjust the NCO frequency. When locked, the NCO exactly replicates the transmit
// carrier and the data appears on the I channel.
//
// Costas loop dynamics:
//   error(t) = I(t) · Q(t)  [sign(I)·Q for hard decisions, but soft is better]
//   frequency += loop_gain * error
//   phase     += frequency
//
// This is a second-order loop (integrating frequency): it can track a constant
// frequency offset. For a third-order loop capable of tracking frequency ramp,
// add a second integrator on the frequency path.
//
// Matched filter: integrate input over one symbol period, then dump (reset).
// In Faust this is approximated as a running average lowpass at symbol_rate/2.
//
// Inputs:  2 — [0] I from SDR (baseband), [1] Q from SDR (baseband)
// Outputs: 1 — [0] soft symbol decisions (+1.0 ≈ bit 1, -1.0 ≈ bit 0)
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/bpsk_demod.dsp -o build/bpsk_demod_gen.cpp

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
symbol_rate  = hslider("/bpsk_demod/symbol_rate",  9600, 300, 115200, 1) : si.smoo;
// Costas loop bandwidth (normalised to symbol rate). Lower = more noise
// rejection, slower acquisition. 0.01–0.05 is typical for BPSK.
loop_bw      = hslider("/bpsk_demod/loop_bw",      0.02, 0.001, 0.1, 0.001);
// Matched filter bandwidth: set to ~symbol_rate/2 for rectangular MF
mf_bw        = hslider("/bpsk_demod/mf_bw",        0.5,  0.1,  1.0, 0.01);
output_gain  = hslider("/bpsk_demod/output_gain",   1.0,  0.0,  4.0, 0.01);

// ── Costas loop ───────────────────────────────────────────────────────────────
// Loop proportional + integral gains (from loop bandwidth and damping ζ=0.707)
//   Kp = (4 * ζ * loop_bw) / (1 + 2*ζ*loop_bw + loop_bw²)
//   Ki = (4 * loop_bw²)    / (1 + 2*ζ*loop_bw + loop_bw²)
// For small loop_bw these simplify to Kp ≈ 4·ζ·BL, Ki ≈ 4·BL²
zeta         = 0.707;  // critically damped (Butterworth damping)
kp           = 4.0 * zeta * loop_bw;
ki           = 4.0 * loop_bw * loop_bw;

// NCO driven by accumulated frequency correction
// phase accumulates at: base_phase + integral(freq_correction)
// In Faust: use a recursive definition for the integrating VCO
// BPSK Costas PLL — state-transition core.
// Inputs:  (i_s, q_s, ph, fi)  — i/q signal + previous phase/fint
// Outputs: (i_s, q_s, bi, ph_new, fi_new)  — state last so ~ feeds them back
// No with{} binding references any other with{} binding — cycle-free.
pll_core_bpsk(i_s, q_s, ph, fi) = i_s, q_s, bi, ph_new, fi_new
with {
    vi     = cos(ph);
    vq     = sin(ph);
    bi     = i_s * vi + q_s * vq;
    bq     = i_s * vq - q_s * vi;
    e      = bi * bq;
    fi_new = fi + ki * e;
    ph_new = (ph + kp * e + fi_new) : ma.modulo(2.0 * ma.PI);
};

// costas_loop: thread (i_in,q_in) through pll_core_bpsk feedback circuit.
// ~ feeds last 2 outputs (ph_new,fi_new) back to last 2 inputs (ph,fi).
// Pass-through outputs (i,q,bi): discard i,q; return bi.
costas_loop(i_in, q_in) = bb_i
with {
    pll_out = (i_in, q_in) : pll_core_bpsk ~ (_, _);
    bb_i    = pll_out : !, !, _, !, !;
};
  // I arm carries the demodulated data

// ── Matched filter (63-tap RRC receive filter) ────────────────────────────────
//
// The optimal receiver for an RRC-shaped transmit signal is a matched filter
// with the same RRC impulse response — giving a raised-cosine end-to-end
// response and zero ISI at the correct sampling instants.
//
// Same coefficients as the transmit filter (alpha=0.35, beta=8.0, 63 taps).
// See modulator_hs.dsp for full derivation notes.

rrc35_coeffs = waveform {
  -0.000000499, 0.000000032, 0.000002364, -0.000003902, -0.000001013, 0.000011437, -0.000013271,
  -0.000007479, 0.000037337, -0.000032618, -0.000030961, 0.000099169, -0.000064865, -0.000098284,
  0.000231999, -0.000107568, -0.000269222, 0.000503715, -0.000143010, -0.000685631, 0.001068726,
  -0.000109512, -0.001750678, 0.002395231, 0.000215126, -0.005085017, 0.006802846, 0.001919179,
  -0.024575477, 0.056235955, -0.084361170, 1.095634118, -0.084361170, 0.056235955, -0.024575477,
  0.001919179, 0.006802846, -0.005085017, 0.000215126, 0.002395231, -0.001750678, -0.000109512,
  0.001068726, -0.000685631, -0.000143010, 0.000503715, -0.000269222, -0.000107568, 0.000231999,
  -0.000098284, -0.000064865, 0.000099169, -0.000030961, -0.000032618, 0.000037337, -0.000007479,
  -0.000013271, 0.000011437, -0.000001013, -0.000003902, 0.000002364, 0.000000032, -0.000000499
};

rrc_coeff(n)    = rrc35_coeffs, int(n) : rdtable;
rrc_ntaps       = 63;
matched_filter(sym) = sum(i, rrc_ntaps, rrc_coeff(i) * (sym@i));

// ── Soft decision ─────────────────────────────────────────────────────────────
// Output is a continuous-valued soft symbol ∈ (−∞, +∞).
// Haskell slices this to bits: bit = 1 if sym > 0 else 0.
bpsk_demod(i_in, q_in) = costas_loop(i_in, q_in)
                        : matched_filter
                        : (_ * output_gain);

// ── Main: 2 inputs → 1 output ────────────────────────────────────────────────
process = bpsk_demod;
