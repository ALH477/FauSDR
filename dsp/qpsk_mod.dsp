// qpsk_mod.dsp — DeMoD QPSK Modulator
//
// Quadrature Phase-Shift Keying: 2 bits per symbol, 3 dB spectral efficiency
// gain over BPSK. Used when you need to double throughput in the same
// bandwidth (e.g. 57600 bps in a 40 kHz channel at 28800 baud).
//
// Inputs:  2 — [0] I symbol stream (+/-1.0), [1] Q symbol stream (+/-1.0)
// Outputs: 2 — [0] I RF, [1] Q RF
//
// Constellation (Gray-coded):
//   I=+1 Q=+1 → bits 00  (45°)
//   I=-1 Q=+1 → bits 01  (135°)
//   I=-1 Q=-1 → bits 11  (225°)
//   I=+1 Q=-1 → bits 10  (315°)
//
// Symbol mapping is done upstream in Haskell (DCF.Modem.QPSK).
// Faust handles: pulse shaping + carrier modulation + I/Q split.
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/qpsk_mod.dsp -o build/qpsk_mod_gen.cpp

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
carrier_freq = hslider("/qpsk/carrier_freq", 0.1, 0.001, 0.5, 0.0001) : si.smoo;
symbol_rate  = hslider("/qpsk/symbol_rate",  28800, 300, 115200, 1)    : si.smoo;
rrc_rolloff  = hslider("/qpsk/rrc_rolloff",  0.35,  0.1, 0.9,   0.01)  : si.smoo;
output_gain  = hslider("/qpsk/output_gain",  0.7,   0.0, 1.0,   0.001);

// ── Root-raised-cosine pulse shaping ──────────────────────────────────────────
// Applied independently to I and Q symbol streams before carrier mixing.
// Limits inter-symbol interference across the quadrature pair.
rrc_bw      = symbol_rate * (1.0 + rrc_rolloff) * 0.5;
pulse(sym)  = sym : fi.lowpass(4, rrc_bw);

// ── NCO — quadrature carrier ──────────────────────────────────────────────────
ci = os.oscrc(carrier_freq);   // cos(ωt)
cq = os.oscs(carrier_freq);    // sin(ωt)  [90° behind ci]

// ── QPSK modulation ───────────────────────────────────────────────────────────
// Direct I/Q modulation (not phase modulation — cleaner for QPSK):
//   RF_I(t) = I_shaped(t) · cos(ωt) − Q_shaped(t) · sin(ωt)
//   RF_Q(t) = I_shaped(t) · sin(ωt) + Q_shaped(t) · cos(ωt)
//
// This places the I symbol stream on the I carrier and the Q symbol stream
// on the quadrature carrier, which is exactly what IQ modulation means.
qpsk_mod(i_sym, q_sym) =
  let {
    i_pulse = pulse(i_sym);
    q_pulse = pulse(q_sym);
    rf_i    = i_pulse * ci - q_pulse * cq;
    rf_q    = i_pulse * cq + q_pulse * ci;
  }
  in (rf_i * output_gain), (rf_q * output_gain);

// ── Main process: 2 inputs → 2 outputs ───────────────────────────────────────
process = qpsk_mod;
