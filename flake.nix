{
  description = "DeMoD Faust-SDR Transport — Faust DSP x SoapySDR IQ pipeline";

  inputs = {
    nixpkgs.url      = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url  = "github:numtide/flake-utils";
    rust-overlay.url = "github:oxalica/rust-overlay";
    rust-overlay.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, flake-utils, rust-overlay }:
    # ── System-independent outputs ────────────────────────────────────────────
    #
    # overlays must live OUTSIDE eachDefaultSystem — they are not per-system.
    # Consumers add this to nixpkgs.overlays to get pkgs.haskellPackages.dcf-faust-sdr:
    #
    #   inputs.dcf-faust-sdr.url = "github:ALH477/FauSDR";
    #   nixpkgs.overlays = [ inputs.dcf-faust-sdr.overlays.default ];
    {
      overlays.default = final: prev: {
        haskellPackages = prev.haskellPackages.override {
          overrides = hself: _hsuper: {
            # cabal2nix lowercases pkgconfig-depends names for argument names.
            # cabal file has: pkgconfig-depends: SoapySDR  →  arg name: soapysdr
            # faust/fftw3f/jack2 are NOT in pkgconfig-depends so must NOT be passed.
            dcf-faust-sdr = hself.callCabal2nix "dcf-faust-sdr" ./haskell {
              soapysdr = final.soapysdr-with-plugins;
            };
          };
        };
      };
    }
    //
    # ── Per-system outputs ────────────────────────────────────────────────────
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ rust-overlay.overlays.default ];
          config.allowUnfree = false;
        };

        # ── Haskell package ───────────────────────────────────────────────────
        #
        # cabal2nix generates argument names from pkgconfig-depends (lowercased).
        # The cabal file declares: pkgconfig-depends: SoapySDR
        # So the generated default.nix expects argument name: soapysdr
        # faust/fftw3f/jack2 are extra-libraries, not pkgconfig-depends —
        # they are linked automatically and must NOT be passed here.
        haskellPackages = pkgs.haskellPackages.override {
          overrides = hself: _hsuper: {
            dcf-faust-sdr = hself.callCabal2nix "dcf-faust-sdr" ./haskell {
              soapysdr = pkgs.soapysdr-with-plugins;
            };
          };
        };

        dcfPackage = haskellPackages.dcf-faust-sdr;

        # ── Rust toolchain ────────────────────────────────────────────────────
        rustToolchain = pkgs.rust-bin.stable.latest.default.override {
          extensions = [ "rust-src" "rust-analyzer" "clippy" "rustfmt" ];
          targets = [
            "x86_64-unknown-linux-gnu"
            "thumbv6m-none-eabi"
            "thumbv7em-none-eabihf"
            "riscv32imac-unknown-none-elf"
          ];
        };

        # ── SDR hardware support ──────────────────────────────────────────────
        sdrModules = with pkgs; [
          soapysdr-with-plugins
          rtl-sdr
          hackrf
          uhd
          limesuite
        ];

        # ── DSP and signal processing ─────────────────────────────────────────
        dspStack = with pkgs; [
          faust
          faustlive
          liquid-dsp
          fftwFloat
          volk
          codec2
        ];

        # ── C/C++ toolchain ───────────────────────────────────────────────────
        cppTools = with pkgs; [
          clang_17
          llvm_17
          lld_17
          cmake
          ninja
          pkg-config
          gnumake
          clang-tools_17
        ];

        # ── RF analysis and signal capture ────────────────────────────────────
        analysisTools = with pkgs; [
          gnuradio
          inspectrum
          gqrx
          sox
          baudline
          gdb
          valgrind
          linuxPackages.perf
          hotspot
          hyperfine
          hexyl
          xxd
          socat
          ncat
        ];

        # ── Acoustic modem tools ──────────────────────────────────────────────
        acousticTools = with pkgs; [
          sox          # raw f32 → audio output (quick test without JACK)
          baudline     # spectrum analyser — verify 2/3 kHz tones
          minimodem    # reference modem for comparison / sanity check
          jack2        # JACK audio server
          pipewire     # PipeWire (ArchibaldOS default)
          pw-inspector # PipeWire inspection tool
          patchage     # JACK patchbay GUI
        ];

        # ── Python RF scripting ───────────────────────────────────────────────
        pythonEnv = pkgs.python311.withPackages (ps: with ps; [
          numpy
          scipy
          matplotlib
          pyserial
        ]);

        # ── Shared shell hook ─────────────────────────────────────────────────
        shellHook = ''
          export CC=clang
          export CXX=clang++
          export FAUST_ARCH_PATH="${pkgs.faust}/share/faust"
          export SOAPY_SDR_ROOT="${pkgs.soapysdr-with-plugins}"
          export LLVM_DIR="${pkgs.llvm_17.dev}/lib/cmake/llvm"
          export PKG_CONFIG_PATH="${pkgs.liquid-dsp}/lib/pkgconfig:${pkgs.fftwFloat}/lib/pkgconfig:$PKG_CONFIG_PATH"
          export PROJECT_ROOT="$(pwd)"
          export DEMOD_DSP_DIR="$PROJECT_ROOT/dsp"
          export DEMOD_BUILD_DIR="$PROJECT_ROOT/build"
          export LIBUSB_DEBUG=0

          echo ""
          echo "  ╔═══════════════════════════════════════════════════════╗"
          echo "  ║  DeMoD FauSDR Transport Shell                         ║"
          echo "  ╠═══════════════════════════════════════════════════════╣"
          echo "  ║  Faust $(faust --version 2>&1 | head -1 | awk '{print $NF}')                                   ║"
          echo "  ║  Clang $(clang --version 2>&1 | head -1 | awk '{print $3}')                             ║"
          echo "  ║  Rust  $(rustc --version 2>/dev/null | awk '{print $2}')                             ║"
          echo "  ╚═══════════════════════════════════════════════════════╝"
          echo ""
          echo "  packages.default = dcf-faust-sdr (Haskell library)"
          echo ""
          echo "  RF Modulator DSP (dsp/):"
          echo "    modulator.dsp           BPSK-PM autonomous (standalone)"
          echo "    modulator_hs.dsp        BPSK-PM symbol-stream input (Haskell)"
          echo "    qpsk_mod.dsp            QPSK — 2 bits/symbol"
          echo "    gmsk_mod.dsp            GMSK BT=0.3 — constant envelope"
          echo "    ask_mod.dsp             OOK/ASK — amplitude keying"
          echo "    fsk_mod.dsp             FSK — configurable deviation"
          echo "  RF Demodulator DSP (dsp/):"
          echo "    bpsk_demod.dsp          BPSK Costas loop + matched filter"
          echo "    qpsk_demod.dsp          QPSK Costas loop + decision"
          echo "    gmsk_demod.dsp          GMSK limiter-discriminator"
          echo "    ask_demod.dsp           OOK envelope detector + slicer"
          echo "    fsk_demod.dsp           FSK discriminator (phase derivative)"
          echo "  Audio-band DSP (dsp/):"
          echo "    jack_mod.dsp            BPSK guitar-cable TX (4 kHz, sps=10/20)"
          echo "    jack_demod.dsp          BPSK guitar-cable RX"
          echo "    acoustic_fsk_mod.dsp    FSK acoustic TX (2/3 kHz, 1200 baud)"
          echo "    acoustic_fsk_demod.dsp  FSK acoustic RX (dual-BPF energy)"
          echo ""
          echo "  Aliases:"
          echo "    faust-<scheme>          compile one DSP file  (e.g. faust-qpsk)"
          echo "    faust-all-mod           compile all RF modulators"
          echo "    faust-all-demod         compile all RF demodulators"
          echo "    faust-acoustic-mod      compile acoustic_fsk_mod.dsp"
          echo "    faust-acoustic-demod    compile acoustic_fsk_demod.dsp"
          echo "    faust-jack-mod          compile jack_mod.dsp"
          echo "    faust-jack-demod        compile jack_demod.dsp"
          echo "    sdr-probe               enumerate attached SDR devices"
          echo "    iq-plot <file>          open .cf32 in inspectrum"
          echo "    frame-rx                hex-dump 17-byte DCF frames from stdin"
          echo "    jf-rx                   hex-dump 8-byte JackFrames from stdin"
          echo "    crc-check               CRC-CCITT over stdin bytes (DCF, 15 bytes)"
          echo "    jf-crc-check            CRC-CCITT over stdin bytes (JackFrame, 6 bytes)"
          echo ""

          _fc() {
            local src="$DEMOD_DSP_DIR/$1"
            local arch="$2"
            local out="$DEMOD_BUILD_DIR/$3"
            if [ -z "$arch" ]; then arch="$PROJECT_ROOT/arch/soapy-sdr-lib.cpp"; fi
            mkdir -p "$DEMOD_BUILD_DIR"
            echo "  faust: $1 → $(basename $out)"
            faust -a "$arch" -lang c++ -vec -vs 256 "$src" -o "$out" && echo "  OK" || echo "  FAILED"
          }

          # RF modulators
          alias faust-bpsk='_fc modulator.dsp $PROJECT_ROOT/arch/soapy-sdr.cpp modulator_gen.cpp'
          alias faust-bpsk-hs='_fc modulator_hs.dsp "" modulator_hs_gen.cpp'
          alias faust-qpsk='_fc qpsk_mod.dsp "" qpsk_mod_gen.cpp'
          alias faust-gmsk='_fc gmsk_mod.dsp "" gmsk_mod_gen.cpp'
          alias faust-ask='_fc ask_mod.dsp "" ask_mod_gen.cpp'
          alias faust-fsk='_fc fsk_mod.dsp "" fsk_mod_gen.cpp'

          # RF demodulators
          alias faust-bpsk-demod='_fc bpsk_demod.dsp "" bpsk_demod_gen.cpp'
          alias faust-qpsk-demod='_fc qpsk_demod.dsp "" qpsk_demod_gen.cpp'
          alias faust-gmsk-demod='_fc gmsk_demod.dsp "" gmsk_demod_gen.cpp'
          alias faust-ask-demod='_fc ask_demod.dsp "" ask_demod_gen.cpp'
          alias faust-fsk-demod='_fc fsk_demod.dsp "" fsk_demod_gen.cpp'

          # Audio-band DSP
          alias faust-jack-mod='_fc jack_mod.dsp "" jack_mod_gen.cpp'
          alias faust-jack-demod='_fc jack_demod.dsp "" jack_demod_gen.cpp'
          alias faust-acoustic-mod='_fc acoustic_fsk_mod.dsp "" acoustic_fsk_mod_gen.cpp'
          alias faust-acoustic-demod='_fc acoustic_fsk_demod.dsp "" acoustic_fsk_demod_gen.cpp'

          # Batch compile
          alias faust-all-mod='for m in modulator_hs qpsk_mod gmsk_mod ask_mod fsk_mod; do _fc ''${m}.dsp "" ''${m}_gen.cpp; done'
          alias faust-all-demod='for d in bpsk_demod qpsk_demod gmsk_demod ask_demod fsk_demod; do _fc ''${d}.dsp "" ''${d}_gen.cpp; done'
          alias faust-all-audio='for f in jack_mod jack_demod acoustic_fsk_mod acoustic_fsk_demod; do _fc ''${f}.dsp "" ''${f}_gen.cpp; done'

          # SDR + analysis
          alias sdr-probe='SoapySDRUtil --probe'
          alias iq-plot='inspectrum'
          alias frame-rx='xxd -c 17 -g 1'
          alias jf-rx='xxd -c 8 -g 1'

          alias crc-check='python3 -c "
import sys
d=sys.stdin.buffer.read(15)
crc=0xFFFF
for b in d:
    crc^=b<<8
    for _ in range(8): crc=((crc<<1)^0x1021)&0xFFFF if crc&0x8000 else (crc<<1)&0xFFFF
print(f\"DCF CRC-CCITT: 0x{crc:04X}  wire: {crc>>8:02X} {crc&0xFF:02X}\")
"'

          alias jf-crc-check='python3 -c "
import sys
d=sys.stdin.buffer.read(6)
crc=0xFFFF
for b in d:
    crc^=b<<8
    for _ in range(8): crc=((crc<<1)^0x1021)&0xFFFF if crc&0x8000 else (crc<<1)&0xFFFF
print(f\"JF  CRC-CCITT: 0x{crc:04X}  wire: {crc>>8:02X} {crc&0xFF:02X}\")
print(f\"    pin check: {\\\"PASS\\\" if crc==0xC23F else \\\"FAIL (expected 0xC23F for ref vector)\\\"}\" )
"'

          if [ ! -f .demod-sdr-init ]; then
            echo "  Scaffolding project layout..."
            mkdir -p src arch dsp transport build captures
            touch .demod-sdr-init
          fi
        '';

      in {

        # ── Packages ──────────────────────────────────────────────────────────
        packages = {
          default       = dcfPackage;
          dcf-faust-sdr = dcfPackage;
        };

        # ── Dev shells ────────────────────────────────────────────────────────

        # Full shell: everything including GUI tools and GNU Radio
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr";
          packages = dspStack ++ cppTools ++ analysisTools ++ sdrModules ++ [
            rustToolchain
            pythonEnv
            pkgs.ripgrep
            pkgs.fd
            pkgs.jq
            dcfPackage
          ];
          inherit shellHook;
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # Headless: no GUI, for CI or SSH sessions
        devShells.headless = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr-headless";
          packages = dspStack ++ cppTools ++ sdrModules ++ [
            rustToolchain
            pkgs.python311
            pkgs.hexyl pkgs.xxd pkgs.socat
            dcfPackage
          ];
          inherit shellHook;
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # Modem dev: acoustic FSK focus — sox, baudline, minimodem, JACK, PipeWire
        devShells.modem-dev = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-modem-dev";
          packages = dspStack ++ cppTools ++ sdrModules ++ acousticTools ++ [
            rustToolchain
            pythonEnv
            pkgs.ripgrep
            pkgs.hexyl pkgs.xxd
            dcfPackage
          ];
          inherit shellHook;
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # Embedded cross-compile: RP2040, STM32, RISC-V bare metal
        devShells.embedded = pkgs.mkShell {
          name = "demod-embedded";
          packages = with pkgs; [
            rustToolchain
            gcc-arm-embedded
            openocd
            probe-rs
            cargo-embed
            minicom
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
            echo "           riscv32imac-unknown-none-elf"
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
