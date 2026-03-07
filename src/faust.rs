// src/faust.rs — DeMoD Faust DSP trait and parameter-map utility
// DeMoD LLC | GPL-3.0
//
// Rust equivalent of soapy-sdr-lib.cpp's UI/Meta/MapUI boilerplate.
//
// In the C++ Faust architecture, `<<includeclass>>` injects a `mydsp` class
// that implements the abstract `dsp` base.  In Rust the same contract is
// expressed as the `FaustDsp` trait; any Faust-compiled struct implements it.
//
// The `MapUi` struct mirrors C++ `MapUI`: it collects parameter paths and lets
// callers drive them via `set_param` / `get_param`, exactly as
// `faust_bridge.cpp` does through `faust_dsp_set_param()`.
//
// Faust-generated DSP files (e.g. acoustic_fsk_mod, jack_mod, modulator_hs)
// should implement `FaustDsp` and register their parameters through
// `build_user_interface(&mut MapUi)`.

use std::collections::HashMap;

// ── FaustDsp trait ────────────────────────────────────────────────────────────

/// Core Faust DSP interface, equivalent to the C++ `dsp` abstract base class.
///
/// Implement this trait for every Faust-compiled DSP block.
pub trait FaustDsp {
    /// Initialise internal state for the given sample rate.
    fn init(&mut self, sample_rate: i32);

    /// Number of audio input channels expected by `compute`.
    fn num_inputs(&self) -> i32;

    /// Number of audio output channels produced by `compute`.
    fn num_outputs(&self) -> i32;

    /// Process `count` samples.
    ///
    /// `inputs`  — slice of `num_inputs()` read-only sample buffers, each ≥ `count`  
    /// `outputs` — slice of `num_outputs()` write-only sample buffers, each ≥ `count`
    fn compute(&mut self, count: usize, inputs: &[&[f32]], outputs: &mut [&mut [f32]]);

    /// Register all parameters with `ui` so callers can drive them.
    /// Called once after `init`.
    fn build_user_interface(&mut self, ui: &mut MapUi);
}

// ── MapUi ─────────────────────────────────────────────────────────────────────

/// Flat `path → value` parameter map.
///
/// Mirrors C++ `MapUI`.  The Faust DSP registers pointers to its internal
/// parameter slots via `build_user_interface`; callers then use `set_param` /
/// `get_param` to drive those slots by name.
///
/// # Example
/// ```
/// use demod::faust::MapUi;
///
/// let mut ui = MapUi::new();
/// ui.register("freq", 440.0);
/// ui.set_param("freq", 880.0);
/// assert_eq!(ui.get_param("freq"), Some(880.0));
/// ```
#[derive(Default, Debug)]
pub struct MapUi {
    params: HashMap<String, f32>,
}

impl MapUi {
    pub fn new() -> Self {
        Self::default()
    }

    /// Register a parameter path with its initial value.
    /// Called by `FaustDsp::build_user_interface`.
    pub fn register(&mut self, path: &str, initial: f32) {
        self.params.insert(path.to_owned(), initial);
    }

    /// Set a parameter by path.  Returns `false` if the path is unknown.
    pub fn set_param(&mut self, path: &str, val: f32) -> bool {
        match self.params.get_mut(path) {
            Some(slot) => { *slot = val; true }
            None       => false,
        }
    }

    /// Read a parameter by path.
    pub fn get_param(&self, path: &str) -> Option<f32> {
        self.params.get(path).copied()
    }

    /// Iterate over all registered `(path, value)` pairs.
    pub fn iter(&self) -> impl Iterator<Item = (&str, f32)> {
        self.params.iter().map(|(k, &v)| (k.as_str(), v))
    }
}

// ── Metadata ──────────────────────────────────────────────────────────────────

/// Metadata collector — equivalent to C++ `Meta` base class.
/// Faust-compiled DSP passes `(key, value)` string pairs to `declare` at
/// static-analysis time (e.g. `"name"`, `"author"`, `"version"`).
pub trait FaustMeta {
    fn declare(&mut self, key: &str, value: &str);
}

/// A simple `Vec`-backed metadata store.
#[derive(Default, Debug)]
pub struct MetaVec {
    pub entries: Vec<(String, String)>,
}

impl MetaVec {
    pub fn new() -> Self { Self::default() }
}

impl FaustMeta for MetaVec {
    fn declare(&mut self, key: &str, value: &str) {
        self.entries.push((key.to_owned(), value.to_owned()));
    }
}

// ── Tests ─────────────────────────────────────────────────────────────────────

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn mapui_register_and_get() {
        let mut ui = MapUi::new();
        ui.register("freq", 440.0);
        assert_eq!(ui.get_param("freq"), Some(440.0));
    }

    #[test]
    fn mapui_set_known_param() {
        let mut ui = MapUi::new();
        ui.register("gain", 0.5);
        assert!(ui.set_param("gain", 1.0));
        assert_eq!(ui.get_param("gain"), Some(1.0));
    }

    #[test]
    fn mapui_set_unknown_param_returns_false() {
        let mut ui = MapUi::new();
        assert!(!ui.set_param("nonexistent", 1.0));
    }

    #[test]
    fn metavec_collects_entries() {
        let mut m = MetaVec::new();
        m.declare("name",    "modulator");
        m.declare("version", "1.0");
        assert_eq!(m.entries.len(), 2);
        assert_eq!(m.entries[0], ("name".into(), "modulator".into()));
    }
}
