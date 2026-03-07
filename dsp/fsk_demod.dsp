// fsk_demod.dsp — DeMoD FSK Demodulator
//
// FSK non-coherent demodulation via FM discriminator.
//
// Identical mathematical approach to the GMSK demodulator, but tuned for
// FSK: no Gaussian matched filter on the RX side (since the TX didn't apply
// one), and the post-discriminator filter is a simple integrate-dump
// approximation matched to a rectangular symbol pulse.
//
// Two alternative discriminator modes are provided:
//
//   Phase-derivative (default, "cross-product"):
//     f[n] = I[n]·ΔQ[n] − Q[n]·ΔI[n]   (after limiter normalisation)
//     Advantage: works on baseband I/Q directly, no atan needed.
//     Disadvantage: sensitive to sample rate / frequency normalisation.
//
//   Dual-filter / non-coherent energy discriminator (mode=1):
//     Two bandpass filters centred at mark and space frequencies.
//     Power detector on each output.
//     Decision: choose whichever filter has higher energy.
//     Advantage: robust to frequency offsets, easy to implement.
//     Disadvantage: requires knowing exact mark/space frequencies.
//
// Mode is selected at runtime via /fsk_demod/mode (0=phase-deriv, 1=dual-filter).
//
// Inputs:  2 — [0] I baseband, [1] Q baseband
// Outputs: 1 — [0] soft bit decisions (+ve = mark, -ve = space)
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/fsk_demod.dsp -o build/fsk_demod_gen.cpp

import("stdfaust.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
symbol_rate  = hslider("/fsk_demod/symbol_rate",  9600, 300, 115200, 1)    : si.smoo;
deviation    = hslider("/fsk_demod/deviation_hz", 4800, 100, 50000,  10)   : si.smoo;
// mode: 0 = phase-derivative discriminator, 1 = dual-filter energy detector
mode         = hslider("/fsk_demod/mode",         0,    0,   1,      1);
lim_eps      = hslider("/fsk_demod/lim_eps",      1e-6, 1e-9, 0.01,  1e-9);
output_gain  = hslider("/fsk_demod/output_gain",  1.0,  0.0,  8.0,   0.01);

// ── Limiter ───────────────────────────────────────────────────────────────────
env(i, q)    = sqrt(i*i + q*q) + lim_eps;
li(i, q)     = i / env(i, q);
lq(i, q)     = q / env(i, q);

// ── Mode 0: Phase-derivative discriminator ────────────────────────────────────
phase_disc(i_l, q_l) = i_l * dq - q_l * di
with {
    di = i_l - i_l';
    dq = q_l - q_l';
};


// ── Mode 1: Dual-filter energy discriminator ──────────────────────────────────
// Two 2nd-order bandpass filters centred at mark (carrier + dev) and
// space (carrier - dev) frequencies, normalised.
// dev_norm: deviation expressed as fraction of SR (0..0.5)
dev_norm     = deviation / ma.SR;
// Mark and space centre frequencies (offsets from 0 in baseband)
f_mark       =  dev_norm;
f_space      = -dev_norm;
bpf_bw       = deviation * 0.8;   // filter BW slightly narrower than deviation

// Bandpass filter: second-order peaking shelving approximation
bpf(fc, bw, x) = x : fi.resonbp(fc, bw / fc, 1.0);

energy(x) = x * x : fi.lowpass(1, symbol_rate * 0.5);  // envelope follower

dual_filter_disc(i_l, q_l) = e_mark - e_space
with {
    // Mix to real signal (just use I arm post-limiting for energy comparison)
    real_sig   = i_l;
    e_mark     = energy(bpf(f_mark,  bpf_bw, real_sig));
    e_space    = energy(bpf(f_space, bpf_bw, real_sig));
};
   // + = mark, - = space

// ── Post-discriminator matched filter ────────────────────────────────────────
// Integrate over one symbol period to maximise SNR.
// For rectangular FSK symbols, optimal is integrate-dump.
// Approximated here as a lowpass at symbol_rate/2.
matched(sym) = sym : fi.lowpass(2, symbol_rate * 0.5);

// ── Full demodulator ──────────────────────────────────────────────────────────
fsk_demod(i_in, q_in) = sym * output_gain
with {
    i_l   = li(i_in, q_in);
    q_l   = lq(i_in, q_in);
    disc  = select2(mode > 0.5,
                    phase_disc(i_l, q_l),
                    dual_filter_disc(i_l, q_l));
    sym   = matched(disc);
};


// ── Main: 2 inputs → 1 output ────────────────────────────────────────────────
process = fsk_demod;
