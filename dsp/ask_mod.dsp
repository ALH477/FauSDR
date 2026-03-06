// ask_mod.dsp — DeMoD OOK/ASK Modulator
//
// On-Off Keying / Amplitude-Shift Keying.
//   OOK: carrier fully gated (0 or 1) — special case of ASK with depth = 100%
//   ASK: carrier amplitude varied between two levels (depth < 100%)
//
// Why OOK for DeMoD:
//   - Simplest possible RF modulation — demodulator is a diode + envelope filter
//   - 433 MHz / 315 MHz cheap consumer RF modules (PT2262, SYN480R) are OOK
//   - Useful for the home automation / sensor tier of DeMoD deployments
//   - Low duty cycle → lower average power; relevant for §15.249 field strength
//   - The CC1101 supports OOK natively (just set deviation to 0, power on/off)
//
// Modulation depth:
//   depth = 1.0 → OOK (binary: fully on / fully off)
//   depth = 0.5 → ASK (levels: 1.0 and 0.5)
//   depth < 0.5 → shallow ASK (residual carrier when "off" — better lock)
//
// Inputs:  1 — [0] bit stream (1.0 = mark/on, 0.0 = space/off OR ±1.0 NRZ)
//              Expects 0/1 unipolar by default. Set input_bipolar=1 for NRZ ±1.
// Outputs: 2 — [0] I, [1] Q
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/ask_mod.dsp -o build/ask_mod_gen.cpp

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
carrier_freq   = hslider("/ask/carrier_freq",    0.1,  0.001, 0.5,  0.0001) : si.smoo;
symbol_rate    = hslider("/ask/symbol_rate",     4800, 300,   115200, 1)     : si.smoo;
// Modulation depth: fraction of carrier amplitude removed for "space" symbol
// depth=1.0 → OOK (full on/off), depth=0.5 → 50% ASK
depth          = hslider("/ask/depth",           1.0,  0.1,   1.0,  0.01)   : si.smoo;
// Symbol shaping: raised-cosine envelope to reduce spectral splatter
// 0.0 = square wave (fastest, most spectral splatter)
// 1.0 = fully raised-cosine shaped edges
shape          = hslider("/ask/shape",           0.5,  0.0,   1.0,  0.01)   : si.smoo;
output_gain    = hslider("/ask/output_gain",     0.9,  0.0,   1.0,  0.001);
// Set to 1 if Haskell sends ±1 NRZ; set to 0 if it sends 0/1 unipolar
input_bipolar  = checkbox("/ask/input_bipolar");

// ── Symbol input normalisation ────────────────────────────────────────────────
// Convert NRZ ±1 to unipolar 0/1 if input_bipolar is set
norm(bit) = select2(input_bipolar, bit, (bit + 1.0) * 0.5);

// ── Raised-cosine symbol shaping ─────────────────────────────────────────────
// Low-pass filter at symbol_rate to round the symbol transitions.
// Higher shape → more filtering → softer edge → less BW, more ISI.
shape_bw       = symbol_rate * (1.0 - shape * 0.7);
shaped(bit)    = norm(bit) : fi.lowpass(2, max(shape_bw, symbol_rate * 0.1));

// ── Amplitude envelope ────────────────────────────────────────────────────────
// envelope(t) = 1 − depth · (1 − shaped(bit(t)))
//   bit=1 → envelope = 1.0 (full carrier)
//   bit=0 → envelope = 1 − depth  (silence for OOK, residual for ASK)
envelope(bit)  = 1.0 - depth * (1.0 - shaped(bit));

// ── Carrier ───────────────────────────────────────────────────────────────────
ci = os.oscrc(carrier_freq);
cq = os.oscs(carrier_freq);

// ── ASK modulation ────────────────────────────────────────────────────────────
ask_mod(bit) =
  let {
    env   = envelope(bit);
    i_out = ci * env;
    q_out = cq * env;
  }
  in (i_out * output_gain), (q_out * output_gain);

// ── Main: 1 input → 2 outputs ─────────────────────────────────────────────────
process = ask_mod;
