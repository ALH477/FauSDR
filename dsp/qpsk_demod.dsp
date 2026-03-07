// qpsk_demod.dsp — DeMoD QPSK Demodulator
//
// QPSK coherent demodulation via Costas loop + dual matched filter.
//
// QPSK carries two BPSK streams on quadrature carriers simultaneously.
// After the Costas loop recovers the carrier, the I and Q channels each
// contain an independent BPSK bit stream — each is then matched-filtered
// and sliced independently.
//
// QPSK Costas loop error signal:
//   error = sign(I)·Q − sign(Q)·I
//   (Four-phase Costas loop: removes data modulation from the error term)
//
// At lock:
//   I arm → Gray-decoded bit stream: MSB of each symbol pair
//   Q arm → Gray-decoded bit stream: LSB of each symbol pair
// Haskell re-interleaves them to recover the original bit sequence.
//
// Inputs:  2 — [0] I baseband, [1] Q baseband
// Outputs: 2 — [0] I soft symbols, [1] Q soft symbols
//              Each ≈ +1.0 or -1.0 per symbol; Haskell slices + de-Grays.

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
symbol_rate = hslider("/qpsk_demod/symbol_rate", 28800, 300, 115200, 1) : si.smoo;
loop_bw     = hslider("/qpsk_demod/loop_bw",     0.02,  0.001, 0.1, 0.001);
mf_bw       = hslider("/qpsk_demod/mf_bw",       0.5,   0.1,   1.0, 0.01);
output_gain = hslider("/qpsk_demod/output_gain",  1.0,   0.0,   4.0, 0.01);

// ── Four-phase Costas loop ────────────────────────────────────────────────────
zeta  = 0.707;
kp    = 4.0 * zeta * loop_bw;
ki    = 4.0 * loop_bw * loop_bw;

sgn(x) = select2(x > 0.0, -1.0, 1.0);

// QPSK Costas PLL — state-transition core.
// Outputs: (i_s, q_s, bi, bq, ph_new, fi_new) — state last for ~ feedback.
pll_core_qpsk(i_s, q_s, ph, fi) = i_s, q_s, bi, bq, ph_new, fi_new
with {
    vi     = cos(ph);
    vq     = sin(ph);
    bi     = i_s * vi + q_s * vq;
    bq     = q_s * vi - i_s * vq;
    e      = sgn(bi) * bq - sgn(bq) * bi;
    fi_new = fi + ki * e;
    ph_new = (ph + kp * e + fi_new) : ma.modulo(2.0 * ma.PI);
};

// ~ feeds last 2 outputs (ph_new,fi_new) back to last 2 inputs (ph,fi).
// Pass-through: (i,q,bi,bq) — discard i,q; return bi,bq.
qpsk_costas(i_in, q_in) = bb_i, bb_q
with {
    pll_out = (i_in, q_in) : pll_core_qpsk ~ (_, _);
    bb_i    = pll_out : !, !, _, !, !, !;
    bb_q    = pll_out : !, !, !, _, !, !;
};

// ── Matched filter (63-tap RRC receive filter) ────────────────────────────────
// Applied independently to I and Q channels after Costas loop recovery.
// Same RRC coefficients as the transmit filter — produces zero ISI at correct
// sampling instants when paired with RRC TX shaping (alpha=0.35).

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

rrc_coeff(n) = rrc35_coeffs, int(n) : rdtable;
rrc_ntaps    = 63;
mf(sym)      = sum(i, rrc_ntaps, rrc_coeff(i) * (sym@i));

// ── Full demodulator ──────────────────────────────────────────────────────────
qpsk_demod(i_in, q_in) = i_out, q_out
with {
    costas = qpsk_costas(i_in, q_in);
    i_out  = (costas : _, !) : mf * output_gain;
    q_out  = (costas : !, _) : mf * output_gain;
};


// ── Main: 2 inputs → 2 outputs ───────────────────────────────────────────────
process = qpsk_demod;
