// acoustic_fsk_mod.dsp — DeMoD Acoustic FSK Modulator
//
// Transmits JackFrame data as audible two-tone FSK through any speaker.
// Sounds like a dial-up modem. That is the point.
//
// Architecture:
//   NRZ-L bit stream (+1=mark, -1=space) → continuous-phase FSK → mono audio
//
//   Continuous-phase FSK: a single phasor whose instantaneous frequency
//   is steered by the symbol value. The phase accumulates continuously —
//   no discontinuities at bit transitions, so the transmitted spectrum
//   has no splatter outside the Carson bandwidth.
//
// Tone frequencies:
//   Mark  (bit 1, +1.0) : 2000 Hz  — above speech noise floor
//   Space (bit 0, -1.0) : 3000 Hz  — below laptop speaker rolloff
//   Centre frequency fc : 2500 Hz
//   Deviation Δf        :  500 Hz
//
// Modulation index h = 2Δf / Rs = 1000 / 1200 = 0.833
//   h > 0.5 → tones are orthogonal → clean non-coherent detection
//   h ≈ 1   → classic V.23-style AFSK (used in dial-up, APRS, AX.25)
//
// Carson rule bandwidth: Rs + 2Δf = 1200 + 1000 = 2200 Hz
//   Occupied band: 1400–3600 Hz — fits any laptop speaker or phone speaker.
//
// Symbol rate: 1200 baud
// Sample rate: 48000 Hz (universal — every audio interface supports this)
// Samples per symbol: 48000 / 1200 = 40 (integer — clean timing)
//
// Frame timing (JackFrame, 64 bits):
//   Airtime: 64 / 1200 = 53.3 ms
//   Preamble (16 alt mark/space): 13.3 ms (Costas lock not needed — FSK)
//   Total TX time per frame: ~67 ms
//
// Inputs:  1 — NRZ-L symbol stream from Haskell (±1.0 floats, one per sample,
//              upsampled to sps=40 before this DSP runs)
// Outputs: 1 — mono audio [-1.0, +1.0] → send to JACK/PipeWire playback port
//
// Parameters:
//   /acoustic_fsk_mod/output_gain  — speaker volume (default 0.7, keep < 0.9
//                                    to avoid clipping on cheap speakers)
//   /acoustic_fsk_mod/f_mark       — mark frequency Hz (default 2000)
//   /acoustic_fsk_mod/f_space      — space frequency Hz (default 3000)
//   /acoustic_fsk_mod/symbol_rate  — baud rate (default 1200)
//
// Build:
//   faust -a alsa-gtk.cpp -lang c++ -vec -vs 256 \
//         dsp/acoustic_fsk_mod.dsp -o build/acoustic_fsk_mod_gen.cpp
//
//   Or for JACK arch:
//   faust -a jack-gtk.cpp -lang c++ -vec -vs 256 \
//         dsp/acoustic_fsk_mod.dsp -o build/acoustic_fsk_mod_gen.cpp
//
// DeMoD LLC — GPL-3.0

import("stdfaust.lib");
import("oscillators.lib");
import("filters.lib");
import("maths.lib");

// ── Parameters ────────────────────────────────────────────────────────────────
// f_mark / f_space in Hz. Normalised internally as f / SR.
// Adjustable at runtime — useful if your speaker rolls off below 2 kHz.
f_mark      = hslider("/acoustic_fsk_mod/f_mark",      2000, 200, 8000, 1) : si.smoo;
f_space     = hslider("/acoustic_fsk_mod/f_space",     3000, 200, 8000, 1) : si.smoo;
symbol_rate = hslider("/acoustic_fsk_mod/symbol_rate", 1200, 300, 4800, 1) : si.smoo;
output_gain = hslider("/acoustic_fsk_mod/output_gain", 0.7,  0.0, 1.0, 0.001);

// ── Normalised frequencies ────────────────────────────────────────────────────
// Faust os.phasor / os.oscrc accept frequency in Hz directly — no need to
// normalise manually (ma.SR is available inside Faust).
// We pass Hz directly to phasor.

// ── Continuous-phase FSK via frequency-modulated phasor ──────────────────────
//
// Key property: os.phasor(1, f) is a 0..1 sawtooth whose phase accumulates
// continuously. By modulating f with the symbol value, the phase accumulates
// at the mark or space rate without any discontinuity at bit edges.
//
// inst_freq(bit): bit = +1 → f_mark, bit = -1 → f_space
//   f_inst = fc + bit * deviation
//   where fc = (f_mark + f_space) / 2, deviation = (f_space - f_mark) / 2
//
// Written directly:
//   f_inst = f_mark  when bit = +1
//   f_inst = f_space when bit = -1
//
// select2 is cleaner but introduces a 1-sample discontinuity.
// Interpolation via linear blend: f_inst = fc + bit * dev
// where bit is the ±1 symbol (already bandlimited by sps upsampling).
//
// Since the symbol stream is already upsampled (same value held for sps=40
// samples), the instantaneous frequency switches sharply at bit boundaries.
// The continuous-phase property is maintained by os.phasor carrying over
// its accumulated phase from the previous symbol period.

fc_hz  = (f_mark + f_space) / 2.0;
dev_hz = (f_space - f_mark) / 2.0;    // +500 Hz for default settings

// inst_freq: input is ±1 NRZ symbol, output is instantaneous frequency in Hz
inst_freq(sym) = fc_hz + sym * dev_hz;

// Phasor: accumulates phase at inst_freq — continuous across symbol boundaries
fsk_phasor(sym) = os.phasor(1, inst_freq(sym));

// Convert phasor (0..1) to audio sample via cosine
fsk_sample(sym) = cos(fsk_phasor(sym) * 2.0 * ma.PI);

// ── Preamble tone ─────────────────────────────────────────────────────────────
// The JackFrame sync detection in Haskell looks for the 0xA5 sync byte.
// No additional tone preamble is needed — the frame sync handles it.
// However, a brief silence before transmission lets the mic AGC settle.
// This is handled in Haskell (insert 40-sample silence before each frame).

// ── Output ────────────────────────────────────────────────────────────────────
process = fsk_sample(_) * output_gain;
