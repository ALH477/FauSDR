// src/bin/soapy_sdr.rs — DeMoD SoapySDR IQ transmitter
// DeMoD LLC | GPL-3.0
//
// Rust port of soapy-sdr.cpp.
//
// Encodes DeMoD 17-byte transport frames as BPSK NRZ symbol streams and
// streams them to a SoapySDR-compatible TX device (RTL-SDR, HackRF, LimeSDR, …).
//
// Usage:
//   cargo run --bin soapy_sdr -- [--driver rtlsdr] [--freq 433.92e6] \
//                                [--rate 2e6] [--gain 20] [--burst 1024]
//
// Defaults match the C++ original:
//   driver  = rtlsdr
//   freq    = 433.92 MHz
//   rate    = 2 MSPS
//   gain    = 20 dB
//   burst   = 1024 samples
//
// BPSK NRZ mapping:  bit 1 → +1.0,  bit 0 → −1.0  (MSB first, rectangular pulse)
// Symbol rate:        9 600 Bd  →  ≈208 samples/symbol at 2 MSPS
// Samples per frame:  136 bits × 208 = 28 288 samples
//
// The Faust DSP layer is represented by the `FaustDsp` trait; swap in a
// real Faust-compiled struct to add RRC pulse-shaping and carrier modulation.

use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::time::{Duration, Instant};

use demod::faust::{FaustDsp, MapUi};
use demod::frame::{Frame, FrameType, BROADCAST, FRAME_SIZE};

// ── Default parameters ────────────────────────────────────────────────────────

const DEFAULT_SAMPLE_RATE: f64  = 2_000_000.0;   // 2 MSPS
const DEFAULT_CENTER_FREQ: f64  = 433_920_000.0; // 433.92 MHz
const DEFAULT_TX_GAIN:     f64  = 20.0;          // dB
const DEFAULT_BURST_SIZE:  usize = 1024;          // samples per SoapySDR write
const DEFAULT_DRIVER:      &str = "rtlsdr";

const SYMBOL_RATE: u32 = 9_600; // Baud

// ── Passthrough DSP stub ──────────────────────────────────────────────────────
//
// Replace this with a Faust-compiled struct that implements `FaustDsp` to add
// RRC pulse shaping and I/Q carrier modulation.  The stub passes channel 0
// (the NRZ symbol stream) straight through to outputs[0] (I) with Q = 0.

struct PassthroughDsp {
    _sample_rate: i32,
}

impl FaustDsp for PassthroughDsp {
    fn init(&mut self, sample_rate: i32) { self._sample_rate = sample_rate; }
    fn num_inputs(&self)  -> i32 { 1 }
    fn num_outputs(&self) -> i32 { 2 }

    fn compute(&mut self, count: usize, inputs: &[&[f32]], outputs: &mut [&mut [f32]]) {
        // I = symbol stream verbatim, Q = 0 (no carrier phase modulation yet)
        let n = count.min(inputs[0].len())
                     .min(outputs[0].len())
                     .min(outputs[1].len());
        outputs[0][..n].copy_from_slice(&inputs[0][..n]);
        outputs[1][..n].fill(0.0);
    }

    fn build_user_interface(&mut self, _ui: &mut MapUi) {}
}

// ── BPSK NRZ encoder ─────────────────────────────────────────────────────────

/// Encode a 17-byte frame as a flat NRZ symbol stream.
///
/// Each bit is mapped to `sps` samples: bit=1 → +1.0, bit=0 → −1.0 (MSB first).
/// Total length = 136 × sps samples.
fn bpsk_nrz(frame_bytes: &[u8; FRAME_SIZE], sps: usize) -> Vec<f32> {
    let mut out = Vec::with_capacity(FRAME_SIZE * 8 * sps);
    for &byte in frame_bytes.iter() {
        for bit_pos in (0..8).rev() {
            let sym: f32 = if (byte >> bit_pos) & 1 == 1 { 1.0 } else { -1.0 };
            out.extend(std::iter::repeat(sym).take(sps));
        }
    }
    out
}

// ── Main ─────────────────────────────────────────────────────────────────────

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // ── Ctrl+C handler ────────────────────────────────────────────────────────
    let running = Arc::new(AtomicBool::new(true));
    {
        let r = running.clone();
        ctrlc_install(move || r.store(false, Ordering::SeqCst));
    }

    // ── Parameters (extend with clap/getopts as needed) ───────────────────────
    let sample_rate = DEFAULT_SAMPLE_RATE;
    let center_freq = DEFAULT_CENTER_FREQ;
    let tx_gain     = DEFAULT_TX_GAIN;
    let burst_size  = DEFAULT_BURST_SIZE;
    let driver      = DEFAULT_DRIVER;

    println!(
        "[demod-sdr] driver={driver}  freq={:.3} MHz  rate={:.1} MSPS  gain={tx_gain} dB",
        center_freq / 1e6,
        sample_rate / 1e6,
    );

    let sps = (sample_rate as u32 / SYMBOL_RATE) as usize; // ≈ 208

    // ── Open SoapySDR device ──────────────────────────────────────────────────
    let sdr = soapysdr::Device::new(format!("driver={driver}").as_str())?;
    sdr.set_sample_rate(soapysdr::Direction::Tx, 0, sample_rate)?;
    sdr.set_frequency(soapysdr::Direction::Tx, 0, center_freq, ())?;
    sdr.set_gain(soapysdr::Direction::Tx, 0, tx_gain)?;

    let mut tx = sdr.tx_stream::<num_complex::Complex32>(&[0])?;
    tx.activate(None)?;

    // ── Faust DSP ─────────────────────────────────────────────────────────────
    let mut dsp = PassthroughDsp { _sample_rate: 0 };
    dsp.init(sample_rate as i32);
    assert_eq!(dsp.num_outputs(), 2, "DSP must produce exactly 2 channels (I, Q)");

    // ── Buffers ───────────────────────────────────────────────────────────────
    let mut i_buf: Vec<f32>                    = vec![0.0; burst_size];
    let mut q_buf: Vec<f32>                    = vec![0.0; burst_size];
    let mut iq:    Vec<num_complex::Complex32> = vec![num_complex::Complex32::new(0.0, 0.0); burst_size];

    // ── Frame state ───────────────────────────────────────────────────────────
    let start = Instant::now();
    let mut seq: u16 = 0;

    println!("[demod-sdr] TX running — Ctrl+C to stop");

    // ── Main TX loop ──────────────────────────────────────────────────────────
    while running.load(Ordering::SeqCst) {
        // Build frame
        let timestamp_us = (start.elapsed().as_micros() as u32) & 0x00FF_FFFF;
        let frame = Frame::new(
            1,
            FrameType::Data,
            seq,
            0x0001,
            BROADCAST,
            [0xDE, 0xAD, 0xBE, 0xEF],
            timestamp_us,
        );
        seq = seq.wrapping_add(1);

        let wire: [u8; FRAME_SIZE] = frame.encode();

        // Encode 136 bits as NRZ symbol stream
        let sym_stream = bpsk_nrz(&wire, sps);

        // Feed symbol stream to DSP in burst-sized chunks, write each to SDR
        let total = sym_stream.len();
        let mut offset = 0usize;

        while offset < total && running.load(Ordering::SeqCst) {
            let chunk = (total - offset).min(burst_size);

            // Zero-pad to burst_size so DSP always sees a full block
            i_buf.fill(0.0);
            i_buf[..chunk].copy_from_slice(&sym_stream[offset..offset + chunk]);

            {
                let inputs:      &[&[f32]]        = &[&i_buf];
                let mut out_i = vec![0.0f32; burst_size];
                let mut out_q = vec![0.0f32; burst_size];
                let outputs: &mut [&mut [f32]] = &mut [out_i.as_mut_slice(), out_q.as_mut_slice()];
                dsp.compute(burst_size, inputs, outputs);
                q_buf.copy_from_slice(out_q.as_slice());
                i_buf.copy_from_slice(out_i.as_slice());
            }

            // Interleave I/Q → CF32
            iq.resize(chunk, num_complex::Complex32::new(0.0, 0.0));
            for n in 0..chunk {
                iq[n] = num_complex::Complex32::new(i_buf[n], q_buf[n]);
            }

            let written = tx.write(&[&iq[..chunk]], None, false, Duration::from_secs(1))?;
            if written == 0 {
                eprintln!("[demod-sdr] writeStream returned 0 — aborting");
                break;
            }
            offset += chunk;
        }
    }

    // ── Teardown ──────────────────────────────────────────────────────────────
    tx.deactivate(None)?;
    println!("[demod-sdr] TX stopped cleanly.");
    Ok(())
}

// ── Minimal Ctrl+C shim ───────────────────────────────────────────────────────
// Replace with the `ctrlc` crate for production use.

#[cfg(unix)]
fn ctrlc_install(handler: impl Fn() + Send + 'static) {
    use std::os::raw::c_int;
    static mut HANDLER: Option<Box<dyn Fn() + Send>> = None;
    extern "C" fn sig_handler(_: c_int) {
        // SAFETY: written once before signals are received
        if let Some(h) = unsafe { HANDLER.as_ref() } { h(); }
    }
    unsafe {
        HANDLER = Some(Box::new(handler));
        libc_signal(libc_SIGINT,  sig_handler as usize);
        libc_signal(libc_SIGTERM, sig_handler as usize);
    }
}

#[cfg(unix)]
extern "C" {
    #[link_name = "signal"]
    fn libc_signal(signum: i32, handler: usize) -> usize;
}
#[cfg(unix)] const libc_SIGINT:  i32 = 2;
#[cfg(unix)] const libc_SIGTERM: i32 = 15;

#[cfg(not(unix))]
fn ctrlc_install(_handler: impl Fn() + Send + 'static) {
    // Windows: no-op stub — wire up `SetConsoleCtrlHandler` as needed
}
