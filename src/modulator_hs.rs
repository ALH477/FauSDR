// src/modulator_hs.rs — DeMoD BPSK-PM modulator (Faust-generated)
// DeMoD LLC | GPL-3.0
//
// At build time, build.rs runs:
//   faust -lang rust dsp/modulator_hs.dsp -o $OUT_DIR/faust/modulator_hs_gen.rs
//
// If the .dsp file is present and faust is in PATH, the real mydsp struct is
// compiled in via include!().  Otherwise the stub below is used so the crate
// still builds (useful for CI without an SDR toolchain).
//
// Haskell FFI entry points are the #[no_mangle] extern "C" functions at the
// bottom of this file.  GHC calls them via:
//   foreign import ccall "faust_modulator_create"  faustModulatorCreate  :: IO (Ptr ModulatorDsp)
//   foreign import ccall "faust_modulator_compute"  faustModulatorCompute :: ...

use crate::faust::{FaustDsp, MapUi};

// ── Generated or stub DSP ─────────────────────────────────────────────────────

// When build.rs successfully compiled the .dsp file, include the real mydsp.
// The generated file defines `struct mydsp` implementing `FaustDsp`.
#[cfg(feature = "faust-generated")]
include!(concat!(env!("DEMOD_FAUST_GEN_DIR"), "/modulator_hs_gen.rs"));

// Stub used when the .dsp file isn't compiled yet.
// Passthrough: 1 input (symbol stream) → 2 outputs (I=passthrough, Q=0).
#[cfg(not(feature = "faust-generated"))]
pub struct mydsp {
    sample_rate: i32,
}

#[cfg(not(feature = "faust-generated"))]
impl FaustDsp for mydsp {
    fn init(&mut self, sample_rate: i32)  { self.sample_rate = sample_rate; }
    fn num_inputs(&self)  -> i32 { 1 }
    fn num_outputs(&self) -> i32 { 2 }

    fn compute(&mut self, count: usize, inputs: &[&[f32]], outputs: &mut [&mut [f32]]) {
        let n = count
            .min(inputs[0].len())
            .min(outputs[0].len())
            .min(outputs[1].len());
        outputs[0][..n].copy_from_slice(&inputs[0][..n]);
        outputs[1][..n].fill(0.0);
    }

    fn build_user_interface(&mut self, _ui: &mut MapUi) {}
}

// ── Public constructor ────────────────────────────────────────────────────────

pub fn new_mydsp(sample_rate: i32) -> mydsp {
    let mut dsp = mydsp {
        #[cfg(not(feature = "faust-generated"))]
        sample_rate: 0,
    };
    dsp.init(sample_rate);
    dsp
}

// ── Haskell FFI (ccall) ───────────────────────────────────────────────────────
//
// These symbols are linked by GHC.  Keep the signatures in sync with
// haskell/src/DCF/Faust/Bridge.hs (FaustModulatorDsp).
//
// Memory model: Box::into_raw / Box::from_raw — caller owns the pointer and
// must call faust_modulator_destroy exactly once.

#[no_mangle]
pub extern "C" fn faust_modulator_create(sample_rate: i32) -> *mut mydsp {
    Box::into_raw(Box::new(new_mydsp(sample_rate)))
}

#[no_mangle]
pub extern "C" fn faust_modulator_destroy(dsp: *mut mydsp) {
    if !dsp.is_null() {
        unsafe { drop(Box::from_raw(dsp)); }
    }
}

/// Set a parameter by MapUI path.  Returns 1 on success, 0 if path unknown.
#[no_mangle]
pub extern "C" fn faust_modulator_set_param(
    dsp:   *mut mydsp,
    path:  *const std::ffi::c_char,
    value: f32,
) -> i32 {
    if dsp.is_null() || path.is_null() { return 0; }
    let path_str = unsafe { std::ffi::CStr::from_ptr(path).to_str().unwrap_or("") };
    let mut ui = MapUi::new();
    unsafe { (*dsp).build_user_interface(&mut ui); }
    if ui.set_param(path_str, value) { 1 } else { 0 }
}

/// Compute one block.
///
/// # Safety
/// `in_buf` must point to `count` f32 samples (the symbol stream).
/// `out_i` / `out_q` must each point to a buffer of at least `count` f32s.
#[no_mangle]
pub extern "C" fn faust_modulator_compute(
    dsp:    *mut mydsp,
    count:  usize,
    in_buf: *const f32,
    out_i:  *mut f32,
    out_q:  *mut f32,
) {
    if dsp.is_null() { return; }
    let (input, oi, oq) = unsafe {
        (
            std::slice::from_raw_parts(in_buf, count),
            std::slice::from_raw_parts_mut(out_i, count),
            std::slice::from_raw_parts_mut(out_q, count),
        )
    };
    let inputs:  &[&[f32]]        = &[input];
    let outputs: &mut [&mut [f32]] = &mut [oi, oq];
    unsafe { (*dsp).compute(count, inputs, outputs); }
}
