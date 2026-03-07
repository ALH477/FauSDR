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
          overrides = hself: hsuper: {
            # mtl-evil-instances-0.1 doesn't compile with GHC 9.x / mtl-2.3.
            # All instance declarations need updated superclass constraints.
            # Use a vendored copy with all fixes applied — no sed fragility.
            mtl-evil-instances = hself.callCabal2nix "mtl-evil-instances"
              ./haskell/vendor/mtl-evil-instances {};
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

        # pkgs with allowUnfree — used only by modem-dev shell (baudline)
        pkgsUnfree = import nixpkgs {
          inherit system;
          overlays = [ rust-overlay.overlays.default ];
          config.allowUnfree = true;
        };

        # ── Haskell package ───────────────────────────────────────────────────
        #
        # cabal2nix generates argument names from pkgconfig-depends (lowercased).
        # The cabal file declares: pkgconfig-depends: SoapySDR
        # So the generated default.nix expects argument name: soapysdr
        # faust/fftw3f/jack2 are extra-libraries, not pkgconfig-depends —
        # they are linked automatically and must NOT be passed here.
        #
        # The `jack` Haskell binding requires monad-exception, which requires
        # mtl-evil-instances-0.1.  That package has ancient version bounds
        # (mtl <2.1, transformers <0.3) written in 2012 that are incompatible
        # with GHC 9.6.  Fix: markUnbroken + doJailbreak to strip the bounds.
        # The library itself is trivial and builds fine with current deps.
        haskellPackages = pkgs.haskellPackages.override {
          overrides = hself: hsuper: {
            # mtl-evil-instances-0.1 doesn't compile with GHC 9.x / mtl-2.3.
            # Use a vendored copy with all superclass constraints fixed.
            mtl-evil-instances = hself.callCabal2nix "mtl-evil-instances"
              ./haskell/vendor/mtl-evil-instances {};
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
          llvmPackages_18.clang
          llvmPackages_18.llvm
          llvmPackages_18.lld
          llvmPackages_18.clang-tools
          cmake
          ninja
          pkg-config
          gnumake
        ];

        # ── RF analysis and signal capture ────────────────────────────────────
        analysisTools = with pkgs; [
          gnuradio
          inspectrum
          gqrx
          sox
          gdb
          valgrind
          perf
          hotspot
          hyperfine
          hexyl
          xxd
          socat
          netcat-gnu
        ];

        # ── Acoustic modem tools ──────────────────────────────────────────────
        acousticTools = with pkgs; [
          sox          # raw f32 → audio output (quick test without JACK)
          pkgsUnfree.baudline  # spectrum analyser — verify 2/3 kHz tones (unfree)
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
          export LLVM_DIR="${pkgs.llvmPackages_18.llvm.dev}/lib/cmake/llvm"
          export PKG_CONFIG_PATH="${pkgs.liquid-dsp}/lib/pkgconfig:${pkgs.fftwFloat}/lib/pkgconfig:$PKG_CONFIG_PATH"
          export PROJECT_ROOT="$(pwd)"
          export DEMOD_DSP_DIR="$PROJECT_ROOT/dsp"
          export DEMOD_BUILD_DIR="$PROJECT_ROOT/build"
          export LIBUSB_DEBUG=0

          # ── ANSI palette ───────────────────────────────────────────────────
          _R='\033[0m'
          _B='\033[1m'
          _CY='\033[96m'
          _VT='\033[35m'
          _GR='\033[90m'
          _WH='\033[97m'
          _GN='\033[92m'
          _RD='\033[91m'
          _YL='\033[93m'

          # ── Version probes ─────────────────────────────────────────────────
          _FAUST_VER=$(faust --version 2>&1 | head -1 | awk '{print $NF}')
          _CLANG_VER=$(clang --version 2>&1 | head -1 | awk '{print $3}')
          _RUST_VER=$(rustc --version 2>/dev/null | awk '{print $2}')
          _GHC_VER=$(ghc --version 2>/dev/null | awk '{print $NF}')

          # ── Banner ─────────────────────────────────────────────────────────
          printf "\n"
          printf "  ''${_CY}''${_B}┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓''${_R}\n"
          printf "  ''${_CY}''${_B}┃''${_R}  ''${_B}''${_WH}DeMoD  LLC''${_R}  ''${_VT}Faust-SDR Transport Shell''${_R}              ''${_CY}''${_B}┃''${_R}\n"
          printf "  ''${_CY}''${_B}┃''${_R}  ''${_GR}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━''${_R}  ''${_CY}''${_B}┃''${_R}\n"
          printf "  ''${_CY}''${_B}┃''${_R}  ''${_GR}Faust''${_R}  ''${_WH}%-12s''${_R}  ''${_GR}GHC''${_R}    ''${_WH}%-16s''${_R}  ''${_CY}''${_B}┃''${_R}\n" "$_FAUST_VER" "$_GHC_VER"
          printf "  ''${_CY}''${_B}┃''${_R}  ''${_GR}Clang''${_R}  ''${_WH}%-12s''${_R}  ''${_GR}Rust''${_R}   ''${_WH}%-16s''${_R}  ''${_CY}''${_B}┃''${_R}\n" "$_CLANG_VER" "$_RUST_VER"
          printf "  ''${_CY}''${_B}┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛''${_R}\n"
          printf "\n"

          # ── SDR device probe ───────────────────────────────────────────────
          _SDR_COUNT=$(SoapySDRUtil --find 2>/dev/null | grep -c "driver=" || echo 0)
          if [ "$_SDR_COUNT" -gt 0 ] 2>/dev/null; then
            printf "  ''${_GN}▸''${_R} ''${_WH}%s SDR device(s) found''${_R}\n" "$_SDR_COUNT"
            SoapySDRUtil --find 2>/dev/null \
              | grep "label=" \
              | sed 's/.*label=//;s/,.*$//' \
              | while IFS= read -r dev; do
                  printf "    ''${_GR}·''${_R} %s\n" "$dev"
                done
          else
            printf "  ''${_YL}▸''${_R} ''${_GR}no SDR devices detected  (run ''${_WH}sdr-probe''${_GR} to retry)''${_R}\n"
          fi
          printf "\n"

          # ── nix run targets ────────────────────────────────────────────────
          printf "  ''${_VT}run targets''${_R}\n"
          printf "  ''${_GR}───────────────────────────────────────────────────────''${_R}\n"
          printf "  ''${_CY}nix run .''${_R}           ''${_GR}→''${_R}  demod-sdr-hs       ''${_GR}RF TX beacon''${_R}\n"
          printf "  ''${_CY}nix run .#rx''${_R}        ''${_GR}→''${_R}  demod-rx-hs        ''${_GR}RF RX frame printer''${_R}\n"
          printf "  ''${_CY}nix run .#hello-tx''${_R}  ''${_GR}→''${_R}  acoustic-hello-tx  ''${_GR}type msg → speaker''${_R}\n"
          printf "  ''${_CY}nix run .#hello-rx''${_R}  ''${_GR}→''${_R}  acoustic-hello-rx  ''${_GR}mic → decoded msg''${_R}\n"
          printf "\n"

          # ── DSP inventory with build status ────────────────────────────────
          printf "  ''${_VT}dsp/''${_R}\n"
          printf "  ''${_GR}───────────────────────────────────────────────────────''${_R}\n"
          _dsp_row() {
            local file="$1" desc="$2"
            local stem="''${file%.dsp}"
            if [ -f "$DEMOD_BUILD_DIR/''${stem}_gen.cpp" ]; then
              local built="''${_GN}✓''${_R}"
            else
              local built="''${_GR}·''${_R}"
            fi
            printf "  %b  ''${_CY}%-26s''${_R}  ''${_GR}%s''${_R}\n" "$built" "$file" "$desc"
          }
          printf "  ''${_GR}· RF modulate''${_R}\n"
          _dsp_row modulator_hs.dsp   "BPSK-PM  symbol-stream"
          _dsp_row qpsk_mod.dsp       "QPSK     2 bps Gray-coded"
          _dsp_row gmsk_mod.dsp       "GMSK     BT=0.3 constant envelope"
          _dsp_row ask_mod.dsp        "OOK/ASK  raised-cosine shaping"
          _dsp_row fsk_mod.dsp        "FSK      configurable deviation"
          printf "  ''${_GR}· RF demodulate''${_R}\n"
          _dsp_row bpsk_demod.dsp     "BPSK  Costas + RRC matched filter"
          _dsp_row qpsk_demod.dsp     "QPSK  4-phase Costas + RRC MF"
          _dsp_row gmsk_demod.dsp     "GMSK  limiter-discriminator"
          _dsp_row ask_demod.dsp      "ASK   envelope + adaptive slicer"
          _dsp_row fsk_demod.dsp      "FSK   phase-derivative discriminator"
          printf "  ''${_GR}· audio-band''${_R}\n"
          _dsp_row jack_mod.dsp           "BPSK guitar-cable TX  4 kHz"
          _dsp_row jack_demod.dsp         "BPSK guitar-cable RX"
          _dsp_row acoustic_fsk_mod.dsp   "FSK  acoustic TX  2/3 kHz 1200 baud"
          _dsp_row acoustic_fsk_demod.dsp "FSK  acoustic RX  dual-BPF energy"
          printf "\n"

          # ── Alias cheatsheet ───────────────────────────────────────────────
          printf "  ''${_VT}aliases''${_R}\n"
          printf "  ''${_GR}───────────────────────────────────────────────────────''${_R}\n"
          _al() { printf "  ''${_WH}%-28s''${_R}  ''${_GR}%s''${_R}\n" "$1" "$2"; }
          _al "faust-<scheme>"           "compile one DSP  (e.g. faust-qpsk)"
          _al "faust-all-mod"            "compile all RF modulators"
          _al "faust-all-demod"          "compile all RF demodulators"
          _al "faust-all-audio"          "compile all audio-band DSP"
          _al "sdr-probe"                "enumerate attached SDR devices"
          _al "iq-plot <file.cf32>"      "open capture in inspectrum"
          _al "frame-rx"                 "xxd 17-byte DCF frames from stdin"
          _al "jf-rx"                    "xxd 8-byte JackFrames from stdin"
          _al "crc-check"                "CRC-CCITT stdin[0..14]  (DCF pin 0x42DD)"
          _al "jf-crc-check"             "CRC-CCITT stdin[0..5]   (JF pin 0xC23F)"
          printf "\n"

          # ── Faust compile helper ───────────────────────────────────────────
          _fc() {
            local src="$DEMOD_DSP_DIR/$1"
            local arch="$2"
            local out="$DEMOD_BUILD_DIR/$3"
            if [ -z "$arch" ]; then arch="$PROJECT_ROOT/arch/soapy-sdr-lib.cpp"; fi
            mkdir -p "$DEMOD_BUILD_DIR"
            printf "  ''${_CY}faust''${_R}  %s  ''${_GR}→''${_R}  %s\n" "$1" "$(basename "$out")"
            if faust -a "$arch" -lang c++ -vec -vs 256 "$src" -o "$out" 2>/dev/null; then
              printf "  ''${_GN}✓  compiled''${_R}\n"
            else
              printf "  ''${_RD}✗  FAILED''${_R}\n"
              faust -a "$arch" -lang c++ -vec -vs 256 "$src" -o "$out"
            fi
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
          alias sdr-probe='printf "  ''${_CY}SoapySDR devices''${_R}\n"; SoapySDRUtil --find 2>/dev/null | grep -E "driver=|label=" | sed "s/^/  /" || printf "  ''${_YL}none found''${_R}\n"'
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
pin=\"PASS\" if crc==0x42DD else \"FAIL\"
print(f\"DCF CRC-CCITT: 0x{crc:04X}  wire: {crc>>8:02X} {crc&0xFF:02X}  pin={pin}\")
"'

          alias jf-crc-check='python3 -c "
import sys
d=sys.stdin.buffer.read(6)
crc=0xFFFF
for b in d:
    crc^=b<<8
    for _ in range(8): crc=((crc<<1)^0x1021)&0xFFFF if crc&0x8000 else (crc<<1)&0xFFFF
pin=\"PASS\" if crc==0xC23F else \"FAIL\"
print(f\"JF  CRC-CCITT: 0x{crc:04X}  wire: {crc>>8:02X} {crc&0xFF:02X}  pin={pin}\")
"'

          if [ ! -f .demod-sdr-init ]; then
            printf "  ''${_YL}▸''${_R}  scaffolding project layout...\n"
            mkdir -p src arch dsp transport build captures
            touch .demod-sdr-init
            printf "  ''${_GN}✓''${_R}  src/  arch/  dsp/  transport/  build/  captures/\n\n"
          fi
        '';

      in {

        # ── Packages ──────────────────────────────────────────────────────────
        packages = {
          default       = dcfPackage;
          dcf-faust-sdr = dcfPackage;
        };

        # ── Apps (nix run) ────────────────────────────────────────────────────
        #
        # `nix run .`            → demod-sdr-hs  (TX)
        # `nix run .#rx`         → demod-rx-hs   (RX)
        # `nix run .#hello-tx`   → acoustic-hello-tx
        # `nix run .#hello-rx`   → acoustic-hello-rx
        apps = let
          mkApp = bin: {
            type    = "app";
            program = "${dcfPackage}/bin/${bin}";
          };
        in {
          default    = mkApp "demod-sdr-hs";
          tx         = mkApp "demod-sdr-hs";
          rx         = mkApp "demod-rx-hs";
          hello-tx   = mkApp "acoustic-hello-tx";
          hello-rx   = mkApp "acoustic-hello-rx";
        };

        # ── Dev shells ────────────────────────────────────────────────────────

        # Full shell: everything including GUI tools and GNU Radio
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.llvmPackages_18.stdenv; } {
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
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvmPackages_18.llvm.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvmPackages_18.llvm}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # Headless: no GUI, for CI or SSH sessions
        devShells.headless = pkgs.mkShell.override { stdenv = pkgs.llvmPackages_18.stdenv; } {
          name = "demod-faust-sdr-headless";
          packages = dspStack ++ cppTools ++ sdrModules ++ [
            rustToolchain
            pkgs.python311
            pkgs.hexyl pkgs.xxd pkgs.socat
            dcfPackage
          ];
          inherit shellHook;
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvmPackages_18.llvm.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvmPackages_18.llvm}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # Modem dev: acoustic FSK focus — sox, baudline, minimodem, JACK, PipeWire
        devShells.modem-dev = pkgs.mkShell.override { stdenv = pkgs.llvmPackages_18.stdenv; } {
          name = "demod-modem-dev";
          packages = dspStack ++ cppTools ++ sdrModules ++ acousticTools ++ [
            rustToolchain
            pythonEnv
            pkgs.ripgrep
            pkgs.hexyl pkgs.xxd
            dcfPackage
          ];
          inherit shellHook;
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvmPackages_18.llvm.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvmPackages_18.llvm}/lib -L${pkgs.liquid-dsp}/lib";
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
