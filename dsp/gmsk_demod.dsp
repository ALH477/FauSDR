// gmsk_demod.dsp — DeMoD GMSK Demodulator
//
// GMSK non-coherent demodulation via FM discriminator.
//
// Because GMSK is a form of FM (frequency-modulated), it can be demodulated
// without carrier phase reference — significantly simpler than coherent PSK
// demodulation and tolerant of oscillator drift.
//
// Pipeline:
//   IQ input → Limiter (normalise amplitude) → FM Discriminator (phase derivative)
//            → Gaussian matched filter → soft bit decisions
//
// FM Discriminator (phase derivative method):
//   The instantaneous frequency is the time derivative of the instantaneous phase.
//   For a complex baseband signal s(t) = I(t) + jQ(t):
//     f_inst(t) = (1/2π) · d/dt [arg(s(t))]
//               = (I·dQ/dt − Q·dI/dt) / (I² + Q²)
//
//   In discrete time (first differences):
//     f_disc[n] = (I[n]·(Q[n] − Q[n−1]) − Q[n]·(I[n] − I[n−1])) / r²[n]
//   where r²[n] = I²[n] + Q²[n] (instantaneous power).
//
// The discriminator output is proportional to the deviation frequency.
// For GMSK with h=0.5 and known symbol rate, the soft bits can be integrated
// over one symbol period (or post-filtered) and sliced.
//
// Limiter: divides I and Q by the envelope |s| to normalise amplitude noise
// before the discriminator. Critical for AM-rejection of channel fading.
//
// Inputs:  2 — [0] I baseband, [1] Q baseband
// Outputs: 1 — [0] soft bit decisions (positive = mark, negative = space)
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/gmsk_demod.dsp -o build/gmsk_demod_gen.cpp

import("stdfaust.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
symbol_rate  = hslider("/gmsk_demod/symbol_rate",  9600, 300, 115200, 1) : si.smoo;
bt_product   = hslider("/gmsk_demod/bt_product",   0.3,  0.1, 0.7,   0.01);
// Limiter epsilon: prevents division by zero in silent/fade periods
lim_eps      = hslider("/gmsk_demod/lim_eps",      1e-6, 1e-9, 0.01, 1e-9);
output_gain  = hslider("/gmsk_demod/output_gain",  1.0,  0.0,  8.0,  0.01);

// ── Limiter (AM rejection) ────────────────────────────────────────────────────
// Normalise the complex envelope so the discriminator sees constant amplitude.
// r = √(I² + Q²) + ε   (ε prevents /0 on silence)
envelope(i, q)  = sqrt(i*i + q*q) + lim_eps;
lim_i(i, q)     = i / envelope(i, q);
lim_q(i, q)     = q / envelope(i, q);

// ── FM Discriminator (cross-product instantaneous frequency estimator) ─────────
// f[n] = I[n]·(Q[n]−Q[n−1]) − Q[n]·(I[n]−I[n−1])
// Note: denominator r² = 1 after limiting, so division is omitted.
discriminator(i_lim, q_lim) = i_lim * q_d - q_lim * i_d
with {
    i_d = i_lim - i_lim';    // first difference (dI)
    q_d = q_lim - q_lim';    // first difference (dQ)
};


// ── Post-discriminator Gaussian matched filter ────────────────────────────────
// Matched to the Gaussian filter used in the modulator (BT=bt_product).
// This is the receive filter that maximises SNR at the slicer.
// Approximated as cascaded lowpass to match the transmitter.
post_filter(sym) = sym : fi.lowpass(1, bt_product * symbol_rate)
                       : fi.lowpass(1, bt_product * symbol_rate)
                       : fi.lowpass(1, bt_product * symbol_rate);

// ── Integration filter ────────────────────────────────────────────────────────
// Running average over one symbol period improves SNR before slicing.
integrate(sym) = sym : fi.lowpass(2, symbol_rate * 0.5);

// ── Full demodulator ──────────────────────────────────────────────────────────
gmsk_demod(i_in, q_in) = sym * output_gain
with {
    i_l   = lim_i(i_in, q_in);
    q_l   = lim_q(i_in, q_in);
    disc  = discriminator(i_l, q_l);
    sym   = post_filter(disc) : integrate;
};


// ── Main: 2 inputs → 1 output ────────────────────────────────────────────────
process = gmsk_demod;
