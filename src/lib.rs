// src/lib.rs — DeMoD crate root
// DeMoD LLC | GPL-3.0

pub mod faust;
pub mod frame;

// Generated / placeholder acoustic and JACK modules.
// Populate these with Faust-compiled DSP code as needed.
pub mod acoustic_fsk_demod;
pub mod acoustic_fsk_mod;
pub mod jack_demod;
pub mod jack_mod;
pub mod modulator_hs;

pub use frame::{Frame, FrameError, FrameType, BROADCAST, FRAME_SIZE, SYNC_BYTE};
