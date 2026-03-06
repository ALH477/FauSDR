{
  description = "DeMoD Faust-SDR + Haskell Transport Dev Environment";

  inputs = {
    nixpkgs.url     = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = false;
        };

        # ── SDR device support ────────────────────────────────────────────────
        sdrModules = with pkgs; [
          soapysdr-with-community-support
          rtl-sdr
          hackrf
          uhd
        ];

        # ── DSP + signal processing stack ─────────────────────────────────────
        dspStack = with pkgs; [
          faust
          faustlive
          liquid-dsp
          fftwFloat
          volk
        ];

        # ── C/C++ build toolchain ─────────────────────────────────────────────
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

        # ── Haskell toolchain ─────────────────────────────────────────────────
        # GHC 9.6 (stable, compatible with GHC2021 default-language)
        ghc = pkgs.haskell.packages.ghc96;

        haskellTools = [
          ghc.ghc
          ghc.cabal-install
          ghc.haskell-language-server   # HLS for editor integration
          ghc.hlint
          ghc.ormolu                    # formatter
          ghc.hspec
          ghc.QuickCheck
          # Core libraries (available to cabal via nix)
          ghc.vector
          ghc.binary
          ghc.bytestring
          ghc.time
        ];

        # ── Analysis + debug ──────────────────────────────────────────────────
        analysisTools = with pkgs; [
          gnuradio
          inspectrum
          gqrx
          gdb
          valgrind
          linuxPackages.perf
          hotspot
          hyperfine
        ];

        # ── Python signal analysis ────────────────────────────────────────────
        pythonEnv = pkgs.python311.withPackages (ps: with ps; [
          numpy
          scipy
          matplotlib
          pyserial
        ]);

        # ── Shell hook ────────────────────────────────────────────────────────
        shellHook = ''
          # C/C++ env
          export CC=clang
          export CXX=clang++
          export FAUST_ARCH_PATH="${pkgs.faust}/share/faust"
          export SOAPY_SDR_ROOT="${pkgs.soapysdr-with-community-support}"
          export LLVM_DIR="${pkgs.llvm_17.dev}/lib/cmake/llvm"
          export PKG_CONFIG_PATH="${pkgs.liquid-dsp}/lib/pkgconfig:${pkgs.fftwFloat}/lib/pkgconfig:$PKG_CONFIG_PATH"

          # Haskell env
          export PATH="${ghc.ghc}/bin:${ghc.cabal-install}/bin:$PATH"

          echo ""
          echo "  ╔══════════════════════════════════════════════════════════╗"
          echo "  ║  DeMoD Faust-SDR + Haskell Shell                        ║"
          echo "  ║  GHC   $(ghc --version 2>/dev/null | head -1)            ║"
          echo "  ║  Faust $(faust --version 2>&1 | head -1)                 ║"
          echo "  ╚══════════════════════════════════════════════════════════╝"
          echo ""
          echo "  Build targets:"
          echo "    faust-build   — compile modulator_hs.dsp → C++ (run from haskell/)"
          echo "    cabal-build   — build the Haskell library + executable"
          echo "    cabal-test    — run frame round-trip + QuickCheck tests"
          echo "    sdr-probe     — enumerate attached SDR devices"
          echo ""

          alias faust-build='faust -a $PROJECT_ROOT/arch/soapy-sdr-lib.cpp \
            -lang c++ -vec -vs 256 \
            $PROJECT_ROOT/haskell/dsp/modulator_hs.dsp \
            -o $PROJECT_ROOT/haskell/build/modulator_gen.cpp'
          alias cabal-build='cd $PROJECT_ROOT/haskell && cabal build'
          alias cabal-test='cd $PROJECT_ROOT/haskell && cabal test frame-tests --test-show-details=direct'
          alias sdr-probe='SoapySDRUtil --probe'

          if [ ! -f .demod-sdr-init ]; then
            echo "  → Scaffolding project layout..."
            mkdir -p haskell/build arch dsp captures
            touch .demod-sdr-init
          fi

          export PROJECT_ROOT="$(pwd)"
        '';

      in {
        # ── Primary dev shell ─────────────────────────────────────────────────
        devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr-hs";

          packages = dspStack ++ cppTools ++ analysisTools ++ sdrModules
                  ++ haskellTools ++ [
            pythonEnv
            pkgs.ripgrep
            pkgs.fd
            pkgs.jq
            pkgs.hexyl
            pkgs.xxd
          ];

          inherit shellHook;

          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };

        # ── Headless / CI shell (no GHC GUI tools, no GR) ────────────────────
        devShells.headless = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
          name = "demod-faust-sdr-hs-headless";

          packages = dspStack ++ cppTools ++ sdrModules ++ [
            ghc.ghc
            ghc.cabal-install
            pkgs.python311
            pkgs.hexyl
          ];

          inherit shellHook;

          NIX_CFLAGS_COMPILE = "-I${pkgs.llvm_17.dev}/include";
          NIX_LDFLAGS        = "-L${pkgs.llvm_17}/lib -L${pkgs.liquid-dsp}/lib";
        };
      }
    );
}
