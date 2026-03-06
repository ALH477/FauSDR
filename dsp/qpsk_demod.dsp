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
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/qpsk_demod.dsp -o build/qpsk_demod_gen.cpp

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

// Hard decision (sign function) — used only in the error term, not the output
sgn(x) = select2(x > 0.0, -1.0, 1.0);

qpsk_costas(i_in, q_in) =
  letrec {
    'vco_phase  = (vco_phase + vco_freq) : ma.modulo(2.0 * ma.PI);
    'vco_i      = cos(vco_phase);
    'vco_q      = sin(vco_phase);
    // Rotate received signal by negative VCO phase
    'bb_i       = i_in * vco_i + q_in * vco_q;
    'bb_q       = q_in * vco_i - i_in * vco_q;
    // Four-phase Costas error: removes data from error by using sign decisions
    // error = sign(I)·Q − sign(Q)·I  → zero when phase error is zero
    'phase_err  = sgn(bb_i) * bb_q - sgn(bb_q) * bb_i;
    'freq_int   = freq_int + ki * phase_err;
    'vco_freq   = kp * phase_err + freq_int;
  }
  // Return both I and Q after carrier removal
  in bb_i, bb_q;

// ── Matched filter ────────────────────────────────────────────────────────────
mf(sym) = sym : fi.lowpass(2, mf_bw * symbol_rate * 0.5);

// ── Full demodulator ──────────────────────────────────────────────────────────
qpsk_demod(i_in, q_in) =
  let {
    (i_bb, q_bb) = qpsk_costas(i_in, q_in);
    i_out        = mf(i_bb) * output_gain;
    q_out        = mf(q_bb) * output_gain;
  }
  in i_out, q_out;

// ── Main: 2 inputs → 2 outputs ───────────────────────────────────────────────
process = qpsk_demod;
