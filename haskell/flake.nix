{
  description = "DeMoD Faust-SDR Haskell — GHC + Faust + SoapySDR dev environment";

  inputs = {
    nixpkgs.url     = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
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

        # ── GHC / Haskell toolchain ───────────────────────────────────────────
        # GHC 9.6 LTS — stable, compatible with GHC2021 default-language pragma.
        # Bump to ghc98 when hspec/QuickCheck ecosystem catches up.
        ghc = pkgs.haskell.packages.ghc96;

        haskellTools = [
          ghc.ghc
          pkgs.cabal-install
          ghc.haskell-language-server   # HLS — editor LSP
          ghc.hlint                     # linter
          ghc.ormolu                    # formatter (stricter than stylish-haskell)
          ghc.hspec                     # test framework
          ghc.QuickCheck                # property-based testing
          ghc.hspec-discover            # auto-discover *Spec.hs files
          # Core library dependencies available without cabal fetching from Hackage:
          ghc.vector
          ghc.binary
          ghc.bytestring
          ghc.time
          ghc.stm                       # STM for concurrent frame queues (future RX path)
          ghc.async                     # Async actions for TX/RX concurrency
          ghc.optparse-applicative      # CLI parsing (Main.hs evolution)
        ];

        # ── Rust toolchain (for dcf/rust frame codec + embedded targets) ──────
        rustToolchain = pkgs.rust-bin.stable.latest.default.override {
          extensions = [ "rust-src" "rust-analyzer" "clippy" "rustfmt" ];
          targets = [
            "x86_64-unknown-linux-gnu"
            "thumbv6m-none-eabi"
            "thumbv7em-none-eabihf"
          ];
        };

        # ── SDR hardware ──────────────────────────────────────────────────────
        sdrModules = with pkgs; [
          soapysdr-with-community-support
          rtl-sdr
          hackrf
          uhd
          limesuite
        ];

        # ── DSP stack ─────────────────────────────────────────────────────────
        dspStack = with pkgs; [
          faust
          faustlive
          liquid-dsp
          fftwFloat
          volk
          codec2
        ];

        # ── C/C++ toolchain ───────────────────────────────────────────────────
        # Cabal compiles cbits/ (faust_bridge.cpp, soapy_bridge.cpp) as C++.
        # Clang 17 matches the libfaust LLVM backend version.
        cppTools = with pkgs; [
          clang_17
          llvm_17
          lld_17
          cmake
          ninja
          pkg-config
          clang-tools_17
        ];

        # ── Signal analysis ───────────────────────────────────────────────────
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

        # ── Python (signal analysis + frame verification scripts) ─────────────
        pythonEnv = pkgs.python311.withPackages (ps: with ps; [
          numpy
          scipy
          matplotlib
          pyserial
        ]);

        # ── Shared shell hook ─────────────────────────────────────────────────
        shellHook = ''
          # C/C++ env — Cabal uses this to compile cbits/
          export CC=clang
          export CXX=clang++
          export FAUST_ARCH_PATH="${pkgs.faust}/share/faust"
          export SOAPY_SDR_ROOT="${pkgs.soapysdr-with-community-support}"
          export LLVM_DIR="${pkgs.llvm_17.dev}/lib/cmake/llvm"
          export PKG_CONFIG_PATH="${pkgs.liquid-dsp}/lib/pkgconfig:${pkgs.fftwFloat}/lib/pkgconfig:$PKG_CONFIG_PATH"
          export LIBUSB_DEBUG=0

          # Haskell env
          export PATH="${ghc.ghc}/bin:${pkgs.cabal-install}/bin:$PATH"
          export PROJECT_ROOT="$(pwd)"

          echo ""
          echo "  ╔═══════════════════════════════════════════════════════╗"
          echo "  ║  DeMoD Faust-SDR Haskell Shell                       ║"
          echo "  ╠═══════════════════════════════════════════════════════╣"
          echo "  ║  GHC   $(ghc --version 2>/dev/null | awk '{print $NF}')                                    ║"
          echo "  ║  Cabal $(cabal --version 2>/dev/null | head -1 | awk '{print $3}')                                 ║"
          echo "  ║  Faust $(faust --version 2>&1 | head -1 | awk '{print $NF}')                                   ║"
          echo "  ║  Rust  $(rustc --version 2>/dev/null | awk '{print $2}')                             ║"
          echo "  ╚═══════════════════════════════════════════════════════╝"
          echo ""
          echo "  Haskell modulation modules:"
          echo "    DCF.Transport.Frame    17-byte frame: encode / decode / CRC"
          echo "    DCF.Transport.Symbol   BPSK NRZ-L: frame -> Vector Float"
          echo "    DCF.Modulator          TX pipeline: frame -> Faust -> SoapySDR"
          echo "    DCF.Demodulator        RX pipeline: SoapySDR -> decision -> frame  [TODO]"
          echo "    DCF.Modem.QPSK         QPSK symbol encoder/decoder               [TODO]"
          echo "    DCF.Modem.GMSK         GMSK encoder/Viterbi decoder              [TODO]"
          echo "    DCF.Modem.FSK          FSK encoder/discriminator decoder          [TODO]"
          echo ""
          echo "  Build aliases:"
          echo "    faust-build     compile modulator_hs.dsp -> C++ (run before cabal-build)"
          echo "    cabal-build     build all Haskell targets"
          echo "    cabal-test      run Frame + Symbol + QuickCheck test suite"
          echo "    cabal-run       run demod-sdr-hs with default args"
          echo "    faust-all-audio compile all JACK/acoustic DSP files (run before cabal build acoustic-hello-*)"
          echo "    sdr-probe       enumerate attached SDR devices"
          echo "    iq-plot         open inspectrum (pass .cf32 file as argument)"
          echo "    frame-tx-test   encode the reference frame and hex-dump it"
          echo ""

          # Faust DSP compile helper for all modulation schemes
          _faust_hs() {
            local dsp="$1" out="${2:-${dsp%.dsp}_gen.cpp}"
            echo "  faust: $dsp -> $out"
            faust -a "$PROJECT_ROOT/arch/soapy-sdr-lib.cpp" \
                  -lang c++ -vec -vs 256 \
                  "$PROJECT_ROOT/dsp/$dsp" \
                  -o "$PROJECT_ROOT/build/$out" \
              && echo "  OK" || echo "  FAILED"
          }

          # Compile the primary Haskell-integration DSP
          alias faust-build='_faust_hs modulator_hs.dsp modulator_gen.cpp'

          # Per-modulation-scheme aliases — map directly to DCF.Modem.* modules
          alias faust-bpsk='_faust_hs modulator_hs.dsp modulator_hs_gen.cpp'
          alias faust-qpsk='_faust_hs qpsk_mod.dsp qpsk_mod_gen.cpp'
          alias faust-gmsk='_faust_hs gmsk_mod.dsp gmsk_mod_gen.cpp'
          alias faust-ask='_faust_hs ask_mod.dsp ask_mod_gen.cpp'
          alias faust-fsk='_faust_hs fsk_mod.dsp fsk_mod_gen.cpp'

          alias faust-bpsk-demod='_faust_hs bpsk_demod.dsp bpsk_demod_gen.cpp'
          alias faust-qpsk-demod='_faust_hs qpsk_demod.dsp qpsk_demod_gen.cpp'
          alias faust-gmsk-demod='_faust_hs gmsk_demod.dsp gmsk_demod_gen.cpp'
          alias faust-ask-demod='_faust_hs ask_demod.dsp ask_demod_gen.cpp'
          alias faust-fsk-demod='_faust_hs fsk_demod.dsp fsk_demod_gen.cpp'

          alias faust-all-mod='for m in modulator_hs qpsk_mod gmsk_mod ask_mod fsk_mod; do _faust_hs ${m}.dsp; done'
          alias faust-all-demod='for d in bpsk_demod qpsk_demod gmsk_demod ask_demod fsk_demod; do _faust_hs ${d}.dsp; done'
          alias faust-acoustic-mod='_faust_hs acoustic_fsk_mod.dsp acoustic_fsk_mod_gen.cpp'
          alias faust-acoustic-demod='_faust_hs acoustic_fsk_demod.dsp acoustic_fsk_demod_gen.cpp'
          alias faust-jack-mod='_faust_hs jack_mod.dsp jack_mod_gen.cpp'
          alias faust-jack-demod='_faust_hs jack_demod.dsp jack_demod_gen.cpp'
          alias faust-all-audio='for d in jack_mod jack_demod acoustic_fsk_mod acoustic_fsk_demod; do _faust_hs ${d}.dsp; done'

          # Cabal aliases
          alias cabal-build='cd $PROJECT_ROOT && cabal build all'
          alias cabal-test='cd $PROJECT_ROOT && cabal test frame-tests --test-show-details=direct'
          alias cabal-run='cd $PROJECT_ROOT && cabal run demod-sdr-hs --'

          # SDR / analysis
          alias sdr-probe='SoapySDRUtil --probe'
          alias iq-plot='inspectrum'

          # Encode the cross-language reference frame and hex-dump it.
          # Useful for verifying C/Rust/Lisp codec parity from the shell.
          alias frame-tx-test='python3 -c "
d=[0xD3,0x10,0x00,0x01,0x00,0x01,0xFF,0xFF,0xDE,0xAD,0xBE,0xEF,0x00,0x00,0x00]
crc=0xFFFF
for b in d:
    crc^=b<<8
    for _ in range(8): crc=((crc<<1)^0x1021)&0xFFFF if crc&0x8000 else (crc<<1)&0xFFFF
frame=bytes(d)+bytes([crc>>8,crc&0xFF])
print(\"Reference frame (17 bytes):\")
print(\" \".join(f\"{b:02X}\" for b in frame))
print(f\"CRC: 0x{crc:04X}  (expected: 0x42DD)\")
assert crc==0x42DD,f\"CRC mismatch! got 0x{crc:04X}\"
print(\"CRC OK\")
"'

          # Scaffold on first entry
          if [ ! -f .demod-hs-init ]; then
            echo "  Scaffolding Haskell project layout..."
            mkdir -p build dsp src/DCF/{Transport,Faust,SDR,Modem} cbits test app
            touch .demod-hs-init
          fi
        '';

      in {

        # ── Packages: Faust-generated C++ → cabal2nix build ───────────────────
        packages =
          let
            # Step 1: run Faust to produce the generated .cpp files
            faustAudioGen = pkgs.runCommand "faust-audio-gen" {
              buildInputs = [ pkgs.faust ];
            } ''
              mkdir -p $out
              for dsp in acoustic_fsk_mod acoustic_fsk_demod jack_mod jack_demod; do
                faust -a ${self}/arch/jack-lib.cpp \
                      -lang c++ -vec -vs 256 \
                      ${self}/dsp/''${dsp}.dsp \
                      -o $out/''${dsp}_gen.cpp
              done
            '';

            # Step 2: build the Haskell project
            dcfFaustSdr = ghc.callCabal2nix "dcf-faust-sdr" self {
              preConfigure = ''
                mkdir -p build
                cp ${faustAudioGen}/* build/
              '';
            };
          in {
            inherit dcfFaustSdr;

            hello-tx = pkgs.writeShellScriptBin "hello-tx"
              ''exec ${dcfFaustSdr}/bin/acoustic-hello-tx "$@"'';

            hello-rx = pkgs.writeShellScriptBin "hello-rx"
              ''exec ${dcfFaustSdr}/bin/acoustic-hello-rx "$@"'';

            default = dcfFaustSdr;
          };

        # ── Apps: nix run .#hello-tx / .#hello-rx ────────────────────────────
        apps = {
          hello-tx = {
            type    = "app";
            program = "${self.packages.${system}.hello-tx}/bin/hello-tx";
          };
          hello-rx = {
            type    = "app";
            program = "${self.packages.${system}.hello-rx}/bin/hello-rx";
          };
          default = self.apps.${system}.hello-tx;
        };

        # ── default: full Haskell + SDR + analysis shell ──────────────────────
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr-hs";

          packages = haskellTools ++ dspStack ++ cppTools ++ analysisTools
                  ++ sdrModules ++ [
            rustToolchain
            pythonEnv
            pkgs.ripgrep
            pkgs.fd
            pkgs.jq
            pkgs.hexyl
            pkgs.xxd
          ];

          inherit shellHook;

          # clangd needs LLVM headers; Faust cbits need liquid-dsp
          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # ── headless: CI — no GUI, no GNURadio, no gqrx ──────────────────────
        devShells.headless = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr-hs-headless";

          packages = haskellTools ++ dspStack ++ cppTools ++ sdrModules ++ [
            rustToolchain
            pkgs.python311
            pkgs.hexyl pkgs.xxd pkgs.socat
          ];

          inherit shellHook;

          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # ── modem-dev: focused shell for modem DSP / Haskell codec work ──────
        # Includes ghci REPL, liquid-dsp, and Python for quick impulse-response
        # plotting — minimal SDR hardware deps so it runs on any dev machine.
        devShells.modem-dev = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-modem-dev";

          packages = haskellTools ++ dspStack ++ cppTools ++ [
            rustToolchain
            pythonEnv
            pkgs.ripgrep
            pkgs.fd
            pkgs.hexyl pkgs.xxd
          ];

          shellHook = shellHook + ''
            echo "  [modem-dev] No SDR hardware dependencies."
            echo "  ghci src/DCF/Modem/QPSK.hs   — load QPSK modem interactively"
            echo "  python3 scripts/plot_impulse.py  — plot filter impulse responses"
            echo ""
          '';

          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };
      }
    );
}
