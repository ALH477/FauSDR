// gmsk_mod.dsp — DeMoD GMSK Modulator
//
// Gaussian Minimum-Shift Keying: constant-envelope FSK with a Gaussian
// pre-modulation filter. Used in GSM, DECT, Bluetooth LE (BLE uses GFSK/BLE
// variant), and anywhere constant envelope and narrow occupied bandwidth both
// matter (power amplifier efficiency + adjacent-channel interference).
//
// Why GMSK for DeMoD:
//   - Constant envelope → class-C PA, ~3 dB more efficient than linear modes
//   - Narrower spectrum than plain MSK due to Gaussian pre-filter
//   - BT=0.3 is the GSM standard; suits 900 MHz §15.249 narrow-band operation
//   - No need for linear upconverter — friendly to cheap SDR hardware
//
// Theory:
//   MSK is BPSK with continuous phase: deviation = ±0.25/symbol, modulation
//   index h = 0.5 (exactly half the symbol rate). GMSK adds a Gaussian low-pass
//   filter (bandwidth-time product BT) before the FM integrator to smooth the
//   phase trajectory, reducing spectral sidelobes at the cost of ISI.
//
//   Phase: φ(t) = π·h · ∫ g(τ) · d(t−τ) dτ
//   where g(t) is the Gaussian filter impulse response.
//
// Faust implementation:
//   1. Input: NRZ bit stream from Haskell (+1/-1 at symbol rate, upsampled)
//   2. Gaussian pre-filter (approximated with cascaded first-order IIR)
//   3. Phase integrator (leaky to prevent unbounded accumulation)
//   4. sin/cos of phase → I/Q RF outputs
//
// Inputs:  1 — [0] NRZ bit stream (+1.0 / -1.0)
// Outputs: 2 — [0] I, [1] Q
//
// Note: the Gaussian filter here is approximated as cascaded lowpass stages.
// A true Gaussian FIR (erf-based coefficients) gives better BT accuracy but
// requires precomputed coefficients — replace for production §15.249 cert.
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/gmsk_mod.dsp -o build/gmsk_mod_gen.cpp

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
carrier_freq = hslider("/gmsk/carrier_freq", 0.1, 0.001, 0.5, 0.0001) : si.smoo;
symbol_rate  = hslider("/gmsk/symbol_rate",  9600, 300, 115200, 1)     : si.smoo;
// BT product: 0.3 = GSM, 0.5 = near-MSK (wider, less ISI), 0.2 = very narrow
bt_product   = hslider("/gmsk/bt_product",   0.3, 0.1, 0.7, 0.01)      : si.smoo;
output_gain  = hslider("/gmsk/output_gain",  0.9, 0.0, 1.0, 0.001);

// ── Gaussian pre-modulation filter ───────────────────────────────────────────
// Cutoff ≈ BT * symbol_rate. Three cascaded first-order sections approximate
// the Gaussian rolloff well enough for a Faust prototype.
gauss_bw(sym)  = sym : fi.lowpass(1, bt_product * symbol_rate)
                     : fi.lowpass(1, bt_product * symbol_rate)
                     : fi.lowpass(1, bt_product * symbol_rate);

// ── FM phase integrator ───────────────────────────────────────────────────────
// Modulation index h = 0.5 for MSK/GMSK (half-wave deviation per symbol).
// Phase increment per sample = π * h * symbol / samples_per_symbol
// ≈ π * 0.5 * filtered_bit / (SR / symbol_rate)
// In normalized form: π * 0.5 * filtered_bit * symbol_rate / SR
mod_index = 0.5;   // h = 0.5 is MSK; fixed for GMSK

// Integrate filtered symbols to get instantaneous phase.
// Leaky integrator (pole at 0.9999) prevents unbounded drift.
// The phase is in [−π, +π] range under normal operation.
phase_incr(sym) = gauss_bw(sym) * (ma.PI * mod_index * symbol_rate / ma.SR);
phase(sym)      = phase_incr(sym) : fi.pole(0.9999);

// ── I/Q from phase ────────────────────────────────────────────────────────────
// I(t) = cos(ω_c·t + φ(t)),  Q(t) = sin(ω_c·t + φ(t))
//
// os.phasor(tablesize, freq) returns a 0..1 normalized sawtooth.
// tablesize must be a positive integer (use 1 for a normalized unit sawtooth).
// We scale by 2π to get radians, then add the modulation phase φ(t).
gmsk_mod(sym) =
  let {
    phi     = phase(sym);
    // Carrier phasor: 0..1 sawtooth at carrier_freq Hz, scaled to 0..2π radians
    theta_c = os.phasor(1, carrier_freq) * (2.0 * ma.PI);
    // Combine carrier phase with modulation phase (both in radians)
    theta   = theta_c + phi;
    i_out   = cos(theta);
    q_out   = sin(theta);
  }
  in (i_out * output_gain), (q_out * output_gain);

// ── Main: 1 input → 2 outputs ─────────────────────────────────────────────────
process = gmsk_mod;
