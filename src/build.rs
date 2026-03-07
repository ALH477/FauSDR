// build.rs — DeMoD Faust DSP code generation
// DeMoD LLC | GPL-3.0
//
// Runs `faust -lang rust <dsp> -o <out>.rs` for every .dsp file in dsp/
// before cargo compiles the crate.  The generated .rs files land in
// $OUT_DIR/faust/ and are include!()'d by each DSP module in src/.
//
// Cargo reruns this script whenever any .dsp file changes.

use std::path::PathBuf;
use std::process::Command;

// DSP files to compile.  Each entry is (src stem, dsp filename).
// All are compiled in library mode (no arch file, no -vec):
//   faust -lang rust dsp/<file>.dsp -o $OUT_DIR/faust/<stem>_gen.rs
const DSP_FILES: &[(&str, &str)] = &[
    ("modulator_hs",       "modulator_hs.dsp"),
    ("acoustic_fsk_mod",   "acoustic_fsk_mod.dsp"),
    ("acoustic_fsk_demod", "acoustic_fsk_demod.dsp"),
    ("jack_mod",           "jack_mod.dsp"),
    ("jack_demod",         "jack_demod.dsp"),
];

fn main() {
    let out_dir  = PathBuf::from(std::env::var("OUT_DIR").unwrap());
    let faust_out = out_dir.join("faust");
    std::fs::create_dir_all(&faust_out).expect("failed to create $OUT_DIR/faust/");

    let manifest_dir = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap());
    let dsp_dir = manifest_dir.join("dsp");

    // Tell cargo to rerun this script if any .dsp file changes
    println!("cargo:rerun-if-changed=dsp/");

    for (stem, filename) in DSP_FILES {
        let src = dsp_dir.join(filename);
        let out = faust_out.join(format!("{stem}_gen.rs"));

        // Skip if .dsp source doesn't exist yet — module will use stub
        if !src.exists() {
            println!("cargo:warning=faust: {filename} not found — {stem} will use stub");
            continue;
        }

        println!("cargo:rerun-if-changed=dsp/{filename}");

        let status = Command::new("faust")
            .args([
                "-lang", "rust",
                src.to_str().unwrap(),
                "-o", out.to_str().unwrap(),
            ])
            .status();

        match status {
            Ok(s) if s.success() => {
                println!("cargo:warning=faust: compiled {filename} → {stem}_gen.rs");
            }
            Ok(s) => {
                // Non-zero exit: emit a warning but don't abort — module uses stub
                println!(
                    "cargo:warning=faust: {filename} failed (exit {}) — {stem} will use stub",
                    s.code().unwrap_or(-1)
                );
            }
            Err(e) => {
                // faust not in PATH — emit warning, don't abort
                println!("cargo:warning=faust: not found in PATH ({e}) — all DSP modules will use stubs");
            }
        }
    }

    // Export OUT_DIR/faust path so modules can find generated files
    println!("cargo:rustc-env=DEMOD_FAUST_GEN_DIR={}", faust_out.display());
}
