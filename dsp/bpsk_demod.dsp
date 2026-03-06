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
costas_loop(i_in, q_in) =
  letrec {
    // VCO phase in [0, 2π)
    'vco_phase = (vco_phase + vco_freq) : ma.modulo(2.0 * ma.PI);
    // VCO outputs
    'vco_i     = cos(vco_phase);
    'vco_q     = sin(vco_phase);
    // Mix incoming signal with local VCO
    'baseband_i = i_in * vco_i + q_in * vco_q;   // I arm (data)
    'baseband_q = i_in * vco_q - q_in * vco_i;   // Q arm (error indicator)
    // Phase error: I*Q — zero when locked (Q → 0), sign indicates direction
    'phase_err  = baseband_i * baseband_q;
    // Loop filter: proportional + integral
    'freq_err_int = freq_err_int + ki * phase_err;          // integrator
    'vco_freq  = kp * phase_err + freq_err_int;             // total correction
  }
  in baseband_i;  // I arm carries the demodulated data

// ── Matched filter (lowpass integrate-dump approximation) ─────────────────────
matched_filter(sym) = sym : fi.lowpass(2, mf_bw * symbol_rate * 0.5);

// ── Soft decision ─────────────────────────────────────────────────────────────
// Output is a continuous-valued soft symbol ∈ (−∞, +∞).
// Haskell slices this to bits: bit = 1 if sym > 0 else 0.
bpsk_demod(i_in, q_in) = costas_loop(i_in, q_in)
                        : matched_filter
                        : (_ * output_gain);

// ── Main: 2 inputs → 1 output ────────────────────────────────────────────────
process = bpsk_demod;
