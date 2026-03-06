{
  description = "DeMoD Faust-SDR Transport — Faust DSP x SoapySDR IQ pipeline";

  inputs = {
    nixpkgs.url      = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url  = "github:numtide/flake-utils";
    rust-overlay.url = "github:oxalica/rust-overlay";
    rust-overlay.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, flake-utils, rust-overlay }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ rust-overlay.overlays.default ];
          config.allowUnfree = false;
        };

        # ── Rust toolchain ────────────────────────────────────────────────────
        # Stable release with cross-compile targets for embedded music hardware.
        # See PLAN.md Stream 5: RP2040 + CC1101 @ 900 MHz.
        rustToolchain = pkgs.rust-bin.stable.latest.default.override {
          extensions = [ "rust-src" "rust-analyzer" "clippy" "rustfmt" ];
          targets = [
            "x86_64-unknown-linux-gnu"
            "thumbv6m-none-eabi"          # RP2040 (Cortex-M0+)
            "thumbv7em-none-eabihf"       # STM32F4/F7 (Cortex-M4/M7 with FPU)
            "riscv32imac-unknown-none-elf" # RISC-V bare metal
          ];
        };

        # ── SDR hardware support ──────────────────────────────────────────────
        sdrModules = with pkgs; [
          soapysdr-with-community-support  # RTL-SDR, HackRF, LimeSDR, PlutoSDR, USRP
          rtl-sdr                          # RTL2832U raw access + udev rules
          hackrf                           # HackRF One TX/RX
          uhd                              # USRP hardware driver
          limesuite                        # LimeSDR support
        ];

        # ── DSP and signal processing ─────────────────────────────────────────
        dspStack = with pkgs; [
          faust                 # Faust compiler: faust, faust2*, faust2pd
          faustlive             # Live JIT Faust environment
          liquid-dsp            # Reference DSP: NCO, filters, modems, equalizers
          fftwFloat             # FFTW3 single-precision: FFT, correlation, convolution
          volk                  # SIMD-accelerated DSP kernels
          codec2                # Codec2 / FreeDV: open HF voice codec
        ];

        # ── C/C++ toolchain ───────────────────────────────────────────────────
        # Clang 17 matches the LLVM version embedded in libfaust, preventing
        # ABI mismatches when linking Faust-generated C++ against libfaust JIT.
        cppTools = with pkgs; [
          clang_17
          llvm_17
          lld_17
          cmake
          ninja
          pkg-config
          gnumake
          clang-tools_17        # clangd LSP, clang-format, clang-tidy
        ];

        # ── RF analysis and signal capture ────────────────────────────────────
        analysisTools = with pkgs; [
          gnuradio              # Flowgraph IDE, spectrum sink, OOT block sandbox
          inspectrum            # IQ file viewer (.cf32 .cs16 .cu8)
          gqrx                  # SDR receiver + waterfall GUI
          sox                   # Audio swiss-army knife; WAV<->raw conversion
          baudline              # High-resolution time/frequency display
          gdb
          valgrind
          linuxPackages.perf
          hotspot               # perf flamegraph viewer
          hyperfine             # CLI benchmark harness
          hexyl                 # Colourised hex dump — frame inspection
          xxd
          socat                 # IPC testing: named-pipe loopback for frame exchange
          ncat                  # Frame injection over UDP/TCP for integration tests
        ];

        # ── Python RF scripting ───────────────────────────────────────────────
        pythonEnv = pkgs.python311.withPackages (ps: with ps; [
          numpy
          scipy
          matplotlib
          pyserial              # UART/USB serial for hardware bringup
        ]);

        # ── Shared shell hook ─────────────────────────────────────────────────
        shellHook = ''
          export CC=clang
          export CXX=clang++
          export FAUST_ARCH_PATH="${pkgs.faust}/share/faust"
          export SOAPY_SDR_ROOT="${pkgs.soapysdr-with-community-support}"
          export LLVM_DIR="${pkgs.llvm_17.dev}/lib/cmake/llvm"
          export PKG_CONFIG_PATH="${pkgs.liquid-dsp}/lib/pkgconfig:${pkgs.fftwFloat}/lib/pkgconfig:$PKG_CONFIG_PATH"
          export PROJECT_ROOT="$(pwd)"
          export DEMOD_DSP_DIR="$PROJECT_ROOT/dsp"
          export DEMOD_BUILD_DIR="$PROJECT_ROOT/build"
          export LIBUSB_DEBUG=0   # suppress RTL-SDR USB verbosity during dev

          echo ""
          echo "  ╔═══════════════════════════════════════════════════════╗"
          echo "  ║  DeMoD FauSDR Transport Shell                         ║"
          echo "  ╠═══════════════════════════════════════════════════════╣"
          echo "  ║  Faust $(faust --version 2>&1 | head -1 | awk '{print $NF}')                                   ║"
          echo "  ║  Clang $(clang --version 2>&1 | head -1 | awk '{print $3}')                             ║"
          echo "  ║  Rust  $(rustc --version 2>/dev/null | awk '{print $2}')                             ║"
          echo "  ╚═══════════════════════════════════════════════════════╝"
          echo ""
          echo "  Modulator DSP files (dsp/):"
          echo "    modulator.dsp       BPSK-PM autonomous (no ext input)"
          echo "    modulator_hs.dsp    BPSK-PM symbol-stream input (Haskell)"
          echo "    qpsk_mod.dsp        QPSK — 2 bits/symbol, +3dB spectral efficiency"
          echo "    gmsk_mod.dsp        GMSK BT=0.3 — constant envelope, phone-grade"
          echo "    ask_mod.dsp         OOK/ASK — simple amplitude keying"
          echo "    fsk_mod.dsp         FSK — configurable deviation"
          echo "  Demodulator DSP files (dsp/):"
          echo "    bpsk_demod.dsp      BPSK Costas loop + matched filter"
          echo "    qpsk_demod.dsp      QPSK Costas loop + decision"
          echo "    gmsk_demod.dsp      GMSK limiter-discriminator"
          echo "    ask_demod.dsp       OOK envelope detector + slicer"
          echo "    fsk_demod.dsp       FSK discriminator (phase derivative)"
          echo ""
          echo "  Aliases:"
          echo "    faust-<scheme>      compile one DSP file  (e.g. faust-qpsk)"
          echo "    faust-all-mod       compile all modulators"
          echo "    faust-all-demod     compile all demodulators"
          echo "    sdr-probe           enumerate attached SDR devices"
          echo "    iq-plot <file>      open .cf32 in inspectrum"
          echo "    frame-rx            hex-dump 17-byte frames from stdin"
          echo "    crc-check           compute DCF CRC-CCITT over stdin bytes"
          echo ""

          # Faust compile helper
          _fc() {
            local src="$DEMOD_DSP_DIR/$1" arch="${2:-$PROJECT_ROOT/arch/soapy-sdr-lib.cpp}" out="$DEMOD_BUILD_DIR/$3"
            mkdir -p "$DEMOD_BUILD_DIR"
            echo "  faust: $1 → $(basename $out)"
            faust -a "$arch" -lang c++ -vec -vs 256 "$src" -o "$out" && echo "  OK" || echo "  FAILED"
          }

          # Modulator compile aliases
          alias faust-bpsk='_fc modulator.dsp $PROJECT_ROOT/arch/soapy-sdr.cpp modulator_gen.cpp'
          alias faust-bpsk-hs='_fc modulator_hs.dsp "" modulator_hs_gen.cpp'
          alias faust-qpsk='_fc qpsk_mod.dsp "" qpsk_mod_gen.cpp'
          alias faust-gmsk='_fc gmsk_mod.dsp "" gmsk_mod_gen.cpp'
          alias faust-ask='_fc ask_mod.dsp "" ask_mod_gen.cpp'
          alias faust-fsk='_fc fsk_mod.dsp "" fsk_mod_gen.cpp'

          # Demodulator compile aliases
          alias faust-bpsk-demod='_fc bpsk_demod.dsp "" bpsk_demod_gen.cpp'
          alias faust-qpsk-demod='_fc qpsk_demod.dsp "" qpsk_demod_gen.cpp'
          alias faust-gmsk-demod='_fc gmsk_demod.dsp "" gmsk_demod_gen.cpp'
          alias faust-ask-demod='_fc ask_demod.dsp "" ask_demod_gen.cpp'
          alias faust-fsk-demod='_fc fsk_demod.dsp "" fsk_demod_gen.cpp'

          # Batch aliases
          alias faust-all-mod='for m in modulator_hs qpsk_mod gmsk_mod ask_mod fsk_mod; do _fc ${m}.dsp "" ${m}_gen.cpp; done'
          alias faust-all-demod='for d in bpsk_demod qpsk_demod gmsk_demod ask_demod fsk_demod; do _fc ${d}.dsp "" ${d}_gen.cpp; done'

          alias sdr-probe='SoapySDRUtil --probe'
          alias iq-plot='inspectrum'
          alias frame-rx='xxd -c 17 -g 1'
          alias crc-check='python3 -c "
import sys,struct
d=sys.stdin.buffer.read(15)
crc=0xFFFF
for b in d:
    crc^=b<<8
    for _ in range(8): crc=((crc<<1)^0x1021)&0xFFFF if crc&0x8000 else (crc<<1)&0xFFFF
print(f\"CRC-CCITT: 0x{crc:04X}  wire: {crc>>8:02X} {crc&0xFF:02X}\")
"'

          # Scaffold on first entry
          if [ ! -f .demod-sdr-init ]; then
            echo "  Scaffolding project layout..."
            mkdir -p src arch dsp transport build captures
            touch .demod-sdr-init
          fi
        '';

      in {

        # ── default: full dev shell ───────────────────────────────────────────
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr";
          packages = dspStack ++ cppTools ++ analysisTools ++ sdrModules ++ [
            rustToolchain
            pythonEnv
            pkgs.ripgrep
            pkgs.fd
            pkgs.jq
          ];
          inherit shellHook;
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # ── headless: CI / remote server — no GUI tools ───────────────────────
        devShells.headless = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr-headless";
          packages = dspStack ++ cppTools ++ sdrModules ++ [
            rustToolchain
            pkgs.python311
            pkgs.hexyl pkgs.xxd pkgs.socat
          ];
          inherit shellHook;
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # ── embedded: RP2040 + CC1101 cross-compile (PLAN.md Stream 5) ───────
        devShells.embedded = pkgs.mkShell {
          name = "demod-embedded";
          packages = with pkgs; [
            rustToolchain
            gcc-arm-embedded   # arm-none-eabi-{gcc,gdb,objcopy,...}
            openocd            # SWD debug probe server
            probe-rs           # Rust-native flashing + RTT logging
            cargo-embed        # probe-rs frontend (reads Embed.toml)
            minicom            # Serial monitor for UART output
            hexyl xxd
          ];
          shellHook = ''
            echo ""
            echo "  ╔═══════════════════════════════════════════════════════╗"
            echo "  ║  DeMoD Embedded Cross-Compile Shell                  ║"
            echo "  ║  Rust $(rustc --version | awk '{print $2}')                                ║"
            echo "  ║  GCC  $(arm-none-eabi-gcc --version | head -1 | awk '{print $NF}')                                 ║"
            echo "  ╚═══════════════════════════════════════════════════════╝"
            echo ""
            echo "  Targets: thumbv6m-none-eabi   (RP2040 Cortex-M0+)"
            echo "           thumbv7em-none-eabihf (STM32F4 Cortex-M4F)"
            echo ""
            echo "  cargo build --target thumbv6m-none-eabi --release"
            echo "  cargo embed --target thumbv6m-none-eabi"
            echo ""
            export CARGO_TARGET_THUMBV6M_NONE_EABI_LINKER="arm-none-eabi-gcc"
            export CARGO_TARGET_THUMBV7EM_NONE_EABIHF_LINKER="arm-none-eabi-gcc"
          '';
        };
      }
    );
}
