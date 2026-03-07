declare name        "RingMod";
declare author      "DeMoD Audio Systems";
declare description "DSB-SC Ring Modulator · Stage 1 of ModDemod pair · educational/live";
declare version     "1.0";
declare license     "GPL-3.0";

// ┌──────────────────────────────────────────────────────────────────────────┐
// │  RingMod — Double-Sideband Suppressed-Carrier Amplitude Modulator        │
// │  DeMoD Audio Systems                                                      │
// │                                                                            │
// │  Architecture:                                                             │
// │    · Carrier:   c[n] = cos(2π·(fc + δ[n])·n/SR)                         │
// │    · Modulate:  y[n] = x[n] · c[n]                                       │
// │    · Spectrum:  if x has component at f_s,                                │
// │                 y has components at |fc + f_s| and |fc − f_s|            │
// │    · Wet/dry:   out  = (1 − mix)·x + mix·y                               │
// │    · OU drift on carrier freq — models VCO instability                   │
// │                                                                            │
// │  Designed to be followed by CoherentDemod.dsp for full mod/demod demo.   │
// └──────────────────────────────────────────────────────────────────────────┘

import("stdfaust.lib");


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  UI PARAMETERS                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════╝

fc    = hslider("h:RingMod/[1] Carrier Freq [unit:Hz][style:knob]",
                440.0, 0.1, 8000.0, 0.1) : si.smoo;

drift = hslider("h:RingMod/[2] Drift [style:knob]",
                0.0, 0.0, 1.0, 0.001) : si.smoo;

wmix  = hslider("h:RingMod/[3] Mix [style:knob]",
                1.0, 0.0, 1.0, 0.001) : si.smoo;

gain  = hslider("h:RingMod/[4] Output Gain [unit:dB][style:knob]",
                0.0, -24.0, 12.0, 0.1) : si.smoo;


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  ORNSTEIN-UHLENBECK DRIFT                                                ║
// ║                                                                          ║
// ║  dX = −θ·X·dt + σ·dW                                                    ║
// ║  Euler-Maruyama discrete:                                                ║
// ║    X[n] = (1 − θ·T)·X[n−1]  +  σ·√T·W[n]                              ║
// ║  θ = 3.0  →  mean-reversion time ≈ 333 ms                               ║
// ║  At max drift (σ·20 Hz), peak OU deviation ≈ ±7 Hz (σ = 1)             ║
// ╚══════════════════════════════════════════════════════════════════════════╝

THETA = 3.0;

ouDrift(sigma) =
  ( +(no.noise * sigma * sqrt(ma.T))
    : *(1.0 - THETA * ma.T)
  ) ~ _;


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  CARRIER OSCILLATOR                                                      ║
// ║                                                                          ║
// ║  Manual phasor (no BLEP overhead at sub-audio rates):                   ║
// ║    φ[n] = (φ[n−1] + (fc + δ[n]) / SR) mod 1                            ║
// ║    c[n] = cos(2π·φ[n])                                                  ║
// ║                                                                          ║
// ║  Note: cos chosen over sin so that a phase offset of 0° in the          ║
// ║  demodulator gives maximum coherent gain (cos(0) = 1).                  ║
// ╚══════════════════════════════════════════════════════════════════════════╝

carrier(f, driftAmt) =
  (f + ouDrift(driftAmt * 20.0)) / float(ma.SR)
  : (+ : ma.decimal) ~ _
  : *(2.0 * ma.PI)
  : cos;


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  RING MODULATOR                                                          ║
// ║                                                                          ║
// ║  y[n] = x[n] · c[n]                                                     ║
// ║  Wet/dry: out = (1 − mix)·x + mix·y                                     ║
// ║                                                                          ║
// ║  At mix=1:  pure ring mod — carrier is suppressed in output.            ║
// ║  At mix=0:  dry pass-through for A/B comparison.                        ║
// ║  Intermediate mix: doubles as AM (adds carrier component).              ║
// ╚══════════════════════════════════════════════════════════════════════════╝

ringMod(x) =
  let { c = carrier(fc, drift); }
  in  x * (1.0 - wmix)  +  x * c * wmix;

gainLin = ma.db2linear(gain);

process = par(i, 2,  ringMod : *(gainLin));
