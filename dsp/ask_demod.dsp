// ask_demod.dsp — DeMoD OOK/ASK Demodulator
//
// OOK/ASK demodulation via envelope detection + adaptive threshold slicer.
//
// This is the simplest demodulator in the family. It doesn't need carrier phase
// reference at all — just the amplitude envelope. A diode rectifier + lowpass
// filter is the analog equivalent; this is the digital baseband version.
//
// Pipeline:
//   IQ input → Envelope detector → Normalise (AGC) → Lowpass (MF)
//            → Adaptive threshold slicer → soft bit output
//
// Envelope detector:
//   e(t) = √(I²(t) + Q²(t))
//   This recovers the amplitude modulation regardless of carrier phase.
//
// Adaptive threshold:
//   The slicer threshold tracks the midpoint between mark and space energy
//   using a slow lowpass. This handles slow AGC drift without manual tuning.
//   threshold(t) ≈ (e_max + e_min) / 2  (approximated via DC estimate)
//
// No carrier recovery needed — OOK is inherently non-coherent.
//
// Inputs:  2 — [0] I baseband, [1] Q baseband
// Outputs: 1 — [0] soft bit output (+ve ≈ 1/mark, -ve ≈ 0/space)
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/ask_demod.dsp -o build/ask_demod_gen.cpp

import("stdfaust.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
symbol_rate  = hslider("/ask_demod/symbol_rate",   4800, 300, 115200, 1)   : si.smoo;
// Threshold adaptation rate: fraction of symbol rate.
// Lower = slower adaptation, more stable for burst signals.
// Higher = faster tracking of level changes (AGC fading, range variation).
adapt_rate   = hslider("/ask_demod/adapt_rate",    0.05, 0.001, 0.5, 0.001);
// Modulation depth (match to transmitter): used for threshold initial estimate
depth        = hslider("/ask_demod/depth",         1.0,  0.1,   1.0, 0.01);
output_gain  = hslider("/ask_demod/output_gain",   1.0,  0.0,   8.0, 0.01);

// ── Envelope detector ────────────────────────────────────────────────────────
// Full-wave AM envelope via Pythagorean magnitude.
envelope(i, q) = sqrt(i*i + q*q);

// ── Matched filter ────────────────────────────────────────────────────────────
// Lowpass at symbol_rate/2 — integrates over one symbol period.
matched(sig) = sig : fi.lowpass(2, symbol_rate * 0.5);

// ── Adaptive DC / threshold tracker ──────────────────────────────────────────
// Estimate the average envelope level with a slow lowpass at adapt_rate * Rs.
// The slicer threshold = average level × (1 − depth/2) normalised to midpoint.
// For OOK (depth=1.0): mark level ≈ E, space level ≈ 0 → threshold ≈ E/2.
dc_estimate(sig) = sig : fi.lowpass(1, adapt_rate * symbol_rate);

// ── Slicer ────────────────────────────────────────────────────────────────────
// Outputs: positive = mark (1), negative = space (0)
// threshold = dc_estimate * (1 - depth * 0.5)
// decision  = envelope - threshold → centred around 0
slicer(env_sig) = env_sig - threshold
with {
    dc        = dc_estimate(env_sig);
    threshold = dc * (1.0 - depth * 0.5);
};


// ── Full demodulator ──────────────────────────────────────────────────────────
ask_demod(i_in, q_in) = out * output_gain
with {
    env = envelope(i_in, q_in);
    mf  = matched(env);
    out = slicer(mf);
};


// ── Main: 2 inputs → 1 output ────────────────────────────────────────────────
process = ask_demod;
