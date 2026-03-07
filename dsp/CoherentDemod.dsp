declare name        "CoherentDemod";
declare author      "DeMoD Audio Systems";
declare description "Coherent Demodulator · Stage 2 of ModDemod pair · educational/live";
declare version     "1.0";
declare license     "GPL-3.0";

// ┌──────────────────────────────────────────────────────────────────────────┐
// │  CoherentDemod — Inverse of RingMod.dsp                                  │
// │  DeMoD Audio Systems                                                      │
// │                                                                            │
// │  Architecture:                                                             │
// │    · Carrier:     c[n] = cos(2π·fc·n/SR  +  Δφ)                         │
// │    · Demodulate:  z[n] = LPF(y[n] · c[n])                               │
// │                                                                            │
// │  Mathematical derivation:                                                 │
// │    y[n] = x[n]·cos(2π·fc·n)          (from RingMod, Δφ_mod = 0)        │
// │    z[n] = y[n]·cos(2π·fc·n + Δφ)                                        │
// │         = x[n]·cos(2π·fc·n)·cos(2π·fc·n + Δφ)                          │
// │    cos A · cos B = ½[cos(A−B) + cos(A+B)], so:                          │
// │         = x[n]·½·[cos(−Δφ) + cos(4π·fc·n + Δφ)]                       │
// │    After LPF (cutoff < 2·fc):                                            │
// │         ≈ x[n]·½·cos(Δφ)                                                │
// │                                                                            │
// │  KEY INSIGHT:                                                             │
// │    Δφ = 0°   → full recovery:  z ≈ x/2  (−6 dB, fixed by makeup gain)  │
// │    Δφ = 90°  → null output:    z ≈ 0    (carrier phase mismatch)        │
// │    Δφ = 180° → inverted:       z ≈ −x/2 (polarity flip)                │
// │                                                                            │
// │  fc and Drift must match RingMod.dsp for perfect signal recovery.        │
// └──────────────────────────────────────────────────────────────────────────┘

import("stdfaust.lib");


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  UI PARAMETERS                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════╝

// ── Carrier (must match RingMod.dsp) ──────────────────────────────────────
fc    = hslider("h:Carrier/[1] Carrier Freq [unit:Hz][style:knob]",
                440.0, 0.1, 8000.0, 0.1) : si.smoo;

drift = hslider("h:Carrier/[2] Drift [style:knob]",
                0.0, 0.0, 1.0, 0.001) : si.smoo;

// ── Demodulator controls ──────────────────────────────────────────────────
// Phase offset Δφ — THE educational parameter.
// Move this away from 0° to demonstrate why coherent detection requires
// carrier phase synchronisation.
phase   = hslider("h:Demod/[3] Phase Offset [unit:°][style:knob]",
                  0.0, 0.0, 360.0, 0.1) : si.smoo;

// LPF cutoff — must be set BELOW 2·fc to reject the double-frequency term.
// Setting it above 2·fc lets you hear the demodulation artifact.
lpf_cut = hslider("h:Demod/[4] LPF Cutoff [unit:Hz][style:knob]",
                  2000.0, 20.0, 8000.0, 1.0) : si.smoo;

// Makeup gain: +6 dB compensates the inherent ×½ from product demod.
makeup  = hslider("h:Demod/[5] Makeup Gain [unit:dB][style:knob]",
                  6.0, -12.0, 12.0, 0.1) : si.smoo;

// Demod bypass — route modulated signal straight to output for comparison.
bypass  = checkbox("h:Demod/[6] Demod Bypass");


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  ORNSTEIN-UHLENBECK DRIFT                                                ║
// ║                                                                          ║
// ║  MUST use identical parameters to RingMod.dsp.                          ║
// ║  When Drift knob is matched but OU processes are independent,            ║
// ║  slight carrier frequency divergence occurs — demonstrating that         ║
// ║  real-world demodulators require phase-locked loops (PLLs).             ║
// ╚══════════════════════════════════════════════════════════════════════════╝

THETA = 3.0;

ouDrift(sigma) =
  ( +(no.noise * sigma * sqrt(ma.T))
    : *(1.0 - THETA * ma.T)
  ) ~ _;


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  LOCAL CARRIER OSCILLATOR                                                ║
// ║                                                                          ║
// ║  c_demod[n] = cos(2π·φ[n] + Δφ)                                        ║
// ║  where φ[n] is the same phasor form as in RingMod.dsp.                  ║
// ║                                                                          ║
// ║  Phase offset Δφ = phase * π/180  (degrees → radians)                  ║
// ╚══════════════════════════════════════════════════════════════════════════╝

phaseRad = phase * (ma.PI / 180.0);

carrierDemod(f, driftAmt) =
  (f + ouDrift(driftAmt * 20.0)) / float(ma.SR)
  : (+ : ma.decimal) ~ _
  : *(2.0 * ma.PI)
  : +(phaseRad)
  : cos;


// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  COHERENT DEMODULATOR                                                    ║
// ║                                                                          ║
// ║  Step 1 — Multiply by local carrier:                                    ║
// ║    z_pre[n] = y[n] · c_demod[n]                                         ║
// ║                                                                          ║
// ║  Step 2 — 2nd-order Butterworth LPF:                                    ║
// ║    z[n] = LPF(z_pre[n], fc_lpf)                                         ║
// ║    Removes the 2·fc double-frequency term.                              ║
// ║    Residual distortion α 1 / (2·fc / fc_lpf) when fc_lpf > 0.          ║
// ║                                                                          ║
// ║  Step 3 — Makeup gain compensates inherent ×½ attenuation:             ║
// ║    out[n] = z[n] · 10^(makeup/20)                                       ║
// ╚══════════════════════════════════════════════════════════════════════════╝

makeupLin = ma.db2linear(makeup);

demod(y) =
  let { c = carrierDemod(fc, drift); }
  in  select2(bypass,
        (y * c) : fi.lowpass(2, max(20.0, lpf_cut)) : *(makeupLin),
        y
      );

process = par(i, 2, demod);
