// fsk_mod.dsp — DeMoD FSK Modulator
//
// Frequency-Shift Keying: data encoded as discrete carrier frequency shifts.
// Binary FSK (2-FSK): mark freq = fc + Δf, space freq = fc − Δf.
// Modulation index h = 2Δf / Rs (Rs = symbol rate).
//
// Why FSK for DeMoD:
//   - Simplest non-trivial modulation — trivial to demodulate with a
//     discriminator (no coherent reference required, handles freq offset well)
//   - Handles poor oscillator phase noise better than PSK
//   - Good for the CC1101 hardware path (CC1101 is natively a GFSK/FSK chip)
//   - h > 1 → orthogonal tones, easy non-coherent detection
//
// This modulator covers h ∈ [0.5, 2.0]:
//   h = 0.5  → MSK (minimum bandwidth, requires coherent detector)
//   h = 1.0  → standard FSK (common for 9600 baud packet radio, APRS)
//   h = 2.0  → wide-deviation FSK (robust, POCSAG-style paging)
//
// Inputs:  1 — [0] NRZ bit stream (+1.0 = mark, -1.0 = space)
// Outputs: 2 — [0] I, [1] Q  (complex analytic signal)
//
// Build:
//   faust -a ../arch/soapy-sdr-lib.cpp -lang c++ -vec -vs 256 \
//         dsp/fsk_mod.dsp -o build/fsk_mod_gen.cpp

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
carrier_freq = hslider("/fsk/carrier_freq",   0.1,  0.001, 0.5,  0.0001) : si.smoo;
symbol_rate  = hslider("/fsk/symbol_rate",    9600, 300,   115200, 1)     : si.smoo;
// Frequency deviation in Hz (= h * symbol_rate / 2)
deviation    = hslider("/fsk/deviation_hz",   4800, 100,   50000,  10)    : si.smoo;
output_gain  = hslider("/fsk/output_gain",    0.9,  0.0,   1.0,    0.001);

// ── Instantaneous frequency ───────────────────────────────────────────────────
// f_inst(t) = carrier_freq ± deviation/SR (normalised)
// bit = +1 → f_inst = carrier + deviation
// bit = -1 → f_inst = carrier - deviation
inst_freq(bit) = carrier_freq + bit * (deviation / ma.SR);

// ── Continuous-phase FSK via frequency modulation ────────────────────────────
// os.phasor(tablesize, freq) produces a 0..1 sawtooth at `freq` Hz.
// tablesize must be a positive integer; the canonical value is 1 for a
// normalized sawtooth. We then scale to 0..2π ourselves.
//
// This gives θ(t) = 2π · ∫ f_inst(τ) dτ, which is continuous-phase FSK.
//
// I(t) = cos(θ(t))
// Q(t) = sin(θ(t))
fsk_mod(bit) =
  let {
    f      = inst_freq(bit);
    phs    = os.phasor(1, f);          // 0..1 sawtooth at instantaneous freq
    theta  = phs * (2.0 * ma.PI);     // scale to 0..2π radians
    i_out  = cos(theta);
    q_out  = sin(theta);
  }
  in (i_out * output_gain), (q_out * output_gain);

// ── Main: 1 input → 2 outputs ─────────────────────────────────────────────────
process = fsk_mod;
