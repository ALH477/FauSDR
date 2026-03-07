# DeMoD — Faust-SDR & Communications Framework

**DeMoD LLC** · lGPL-3.0 · [info@demod.ltd](mailto:info@demod.ltd)

Two codebases. One stack. A Haskell/Faust/SoapySDR RF TX/RX pipeline and a polyglot low-latency communications framework, sharing a custom 17-byte transport protocol at their boundary.

---

<img width="670" height="860" alt="image" src="https://github.com/user-attachments/assets/b51f8d98-2d12-4d85-8012-298776e66be4" />

<img width="736" height="469" alt="image" src="https://github.com/user-attachments/assets/7e7c5eb8-c755-4102-8f1a-3ac4c1d1ceff" />

## Repository Layout

```
demod/
├── flake.nix                    # Root Nix dev shell (demod-faust-sdr-env, -lang rust backend)
├── CMakeLists.txt               # C++ build (arch + transport tests)
├── arch/
│   ├── soapy-sdr.cpp            # Standalone arch with main() (modulator_hs.dsp only)
│   └── soapy-sdr-lib.cpp        # Library arch, no main() (all modem DSP files)
├── dsp/                         # Faust DSP modem library (14 files)
│   ├── modulator_hs.dsp         # BPSK-PM modulator — Haskell FFI path (1-in 2-out IQ)
│   ├── qpsk_mod.dsp             # QPSK modulator  (2 bps, Gray-coded)
│   ├── gmsk_mod.dsp             # GMSK modulator  (constant envelope, BT=0.3)
│   ├── fsk_mod.dsp              # FSK modulator   (h=0.5–2.0, CP via phasor NCO)
│   ├── ask_mod.dsp              # OOK/ASK modulator (raised-cosine shaping)
│   ├── bpsk_demod.dsp           # BPSK demodulator (Costas loop + 63-tap RRC MF)
│   ├── qpsk_demod.dsp           # QPSK demodulator (4-phase Costas + 63-tap RRC MF)
│   ├── gmsk_demod.dsp           # GMSK demodulator (FM discriminator + Gaussian MF)
│   ├── fsk_demod.dsp            # FSK demodulator  (phase-deriv or dual-filter)
│   ├── ask_demod.dsp            # OOK/ASK demodulator (envelope + adaptive slicer)
│   ├── jack_mod.dsp             # Guitar-cable BPSK modulator  (4 kHz, sps=10/20)
│   ├── jack_demod.dsp           # Guitar-cable BPSK demodulator
│   ├── acoustic_fsk_mod.dsp     # Acoustic FSK modulator   (2/3 kHz, 1200 baud)
│   └── acoustic_fsk_demod.dsp  # Acoustic FSK demodulator (dual-BPF energy detector)
├── transport/
│   ├── demod_frame.h            # Legacy packed-struct header (compatibility)
│   ├── dcf_frame.h              # Canonical C99 codec (zero deps, embedded-safe)
│   └── jackframe.h              # 8-byte mini-frame for audio-band links (CRC pin 0xC23F)
├── src/
│   ├── test_frame.cpp           # C++ frame unit tests
│   └── test_conformance.cpp     # Cross-language conformance harness (C, CRC pin 0x42DD)
│
├── haskell/                     # Haskell TX/RX pipeline
│   ├── flake.nix
│   ├── dcf-faust-sdr.cabal      # v0.3.0.0
│   ├── cbits/
│   │   ├── faust_bridge.h/cpp   # extern "C" wrapper for Faust C++ mydsp
│   │   └── soapy_bridge.h/cpp   # extern "C" wrapper for SoapySDR C++ API (TX + RX)
│   ├── src/
│   │   ├── DCF/Faust/FFI.hs     # Raw foreign imports (faust_bridge)
│   │   ├── DCF/Faust/DSP.hs     # Safe bracket-managed DSP handle
│   │   ├── DCF/SDR/SoapyFFI.hs  # Raw foreign imports (TX + RX)
│   │   ├── DCF/SDR/Device.hs    # TX handle (withSdrDevice) + RX handle (withSdrRxDevice)
│   │   ├── DCF/Transport/Frame.hs
│   │   ├── DCF/Transport/Symbol.hs
│   │   ├── DCF/Transport/JackFrame.hs   # 8-byte mini-frame (CRC pin 0xC23F)
│   │   ├── DCF/Modulator.hs     # TX pipeline
│   │   ├── DCF/Demodulator.hs   # RX pipeline (SoapySDR RX → Faust → slicer → decoder)
│   │   └── DCF/Modem/
│   │       ├── QPSK.hs          # Gray-coded QPSK symbol mapper / de-mapper
│   │       ├── Wideband.hs      # Wideband BPSK modem (40–192 kHz, 40/96 kbaud)
│   │       └── AcousticFSK.hs   # Acoustic FSK modem (2/3 kHz, 1200 baud, speaker+mic)
│   ├── app/
│   │   ├── Main.hs              # TX entry point (demod-sdr-hs)
│   │   ├── RxMain.hs            # RX entry point (demod-rx-hs)
│   │   ├── AcousticHelloTx.hs   # Acoustic TX REPL — type message, transmit over speaker
│   │   └── AcousticHelloRx.hs   # Acoustic RX listener — decodes from mic, prints message
│   ├── test/
│   │   ├── Spec.hs
│   │   ├── FrameSpec.hs
│   │   ├── JackFrameSpec.hs     # JackFrame round-trip + CRC pin 0xC23F
│   │   └── ConformanceSpec.hs   # Cross-language CRC pin + reference vector
│   └── dsp/modulator_hs.dsp    # Faust TX DSP (63-tap RRC FIR, 1-in 2-out) → modulator_hs_gen.rs
│
├── hydramesh.lisp               # HydraMesh v2.2.0 UDP SDK + DCF frame adapter
│
├── gr-demod/                    # GNU Radio OOT module
│   ├── CMakeLists.txt
│   └── python/demod/
│       ├── __init__.py
│       ├── dcf_frame_sink.py    # GR block: byte stream → decoded DCF frame messages
│       ├── dcf_frame_source.py  # GR block: message PDUs → DCF frame byte stream
│       └── qa_dcf_frame.py      # pytest test suite (no GR hardware required)
│
└── dcf/                         # DeMoD Communications Framework v5.0.0
    ├── messages.proto
    ├── services.proto
    ├── config.schema.json
    ├── config.json.example
    ├── c_sdk/
    │   ├── include/dcf_sdk/
    │   ├── src/dcf_sdk/
    │   └── plugins/
    │       ├── custom_transport.c   # UDP transport plugin
    │       └── sdr_transport.c      # SDR transport plugin (pipe-based bridge to Faust-SDR)
    ├── rust/src/frame.rs
    ├── lisp/src/d-lisp.lisp
    ├── python/dcf/plugin_manager.py
    └── go/
        ├── go.mod
        ├── frame.go             # Go frame codec (zero deps)
        └── frame_test.go        # Go tests including CRC pin 0x42DD
```

---

## Architecture Overview

### Faust-SDR TX Pipeline

```
┌─────────────────────────────────────────────────────────────────┐
│  Haskell                                                         │
│                                                                  │
│  DeModFrame  ──encodeFrame──►  ByteString (17 bytes)            │
│                                        │                         │
│                             encodeBpsk (BPSK NRZ-L)             │
│                                        │                         │
│                             Vector Float (symbol stream)         │
│                          ┌───── FFI ───┘                        │
└──────────────────────────┼──────────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────────┐
│  Faust  (modulator_hs.dsp → modulator_hs_gen.rs, Rust FFI)       │
│                                                                  │
│  inputs[0]: symbol stream (±1.0 NRZ)                            │
│  → 63-tap Kaiser-windowed RRC FIR (α=0.35, β=8.0)              │
│  → Phase modulation onto NCO carrier                             │
│  outputs[0] = I,  outputs[1] = Q                                │
│                          ┌───── FFI ───┐                        │
└──────────────────────────┼──────────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────────┐
│  SoapySDR  (soapy_bridge.cpp)                                    │
│  Interleaved CF32 → writeStream → RF hardware                   │
└─────────────────────────────────────────────────────────────────┘
```

### Faust-SDR RX Pipeline

```
┌─────────────────────────────────────────────────────────────────┐
│  SoapySDR  (soapy_bridge.cpp — RX path)                          │
│  RF hardware → readStream → Interleaved CF32                    │
│                          ┌───── FFI ───┘                        │
└──────────────────────────┼──────────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────────┐
│  Faust  (bpsk_demod.dsp → bpsk_demod_gen.rs, Rust FFI)           │
│                                                                  │
│  inputs[0]=I, inputs[1]=Q                                        │
│  → Costas loop (2nd-order, ζ=0.707) — carrier recovery          │
│  → 63-tap RRC matched filter (same coefficients as TX FIR)      │
│  outputs[0]: soft symbols ∈ ℝ                                   │
│                          ┌───── FFI ───┐                        │
└──────────────────────────┼──────────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────────┐
│  Haskell  (DCF.Demodulator)                                      │
│                                                                  │
│  sliceSoftSymbols (downsample by sps, threshold at 0)           │
│  bitsToBytes                                                     │
│  findFrame (sliding window sync search)                          │
│  decodeFrame + CRC validate                                      │
│  → Maybe DeModFrame                                              │
└─────────────────────────────────────────────────────────────────┘
```

### Modem DSP Library

| Scheme | TX DSP | RX DSP | TX filter | RX filter |
|--------|--------|--------|-----------|-----------|
| BPSK (RF) | `modulator_hs.dsp` | `bpsk_demod.dsp` | 63-tap RRC FIR | 63-tap RRC MF |
| QPSK | `qpsk_mod.dsp` | `qpsk_demod.dsp` | 63-tap RRC FIR | 63-tap RRC MF |
| GMSK | `gmsk_mod.dsp` | `gmsk_demod.dsp` | 3× cascaded IIR | 3× cascaded IIR |
| FSK (RF) | `fsk_mod.dsp` | `fsk_demod.dsp` | — (CP via phasor) | IIR lowpass |
| OOK/ASK | `ask_mod.dsp` | `ask_demod.dsp` | IIR RC shaping | Envelope + IIR |
| BPSK (guitar cable) | `jack_mod.dsp` | `jack_demod.dsp` | 63/121-tap RRC | 63/121-tap RRC MF |
| FSK (acoustic) | `acoustic_fsk_mod.dsp` | `acoustic_fsk_demod.dsp` | — (CP phasor) | Dual BPF + envelope |

The BPSK and QPSK TX and RX filters are matched: both use the same 63-tap Kaiser-windowed RRC FIR (α=0.35, β=8.0), giving zero ISI at the correct sampling instants (raised-cosine end-to-end response).

---

## The 17-Byte Transport Frame

```
 0       1       2-3     4-5     6-7     8-11    12-14   15-16
┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┐
│ SYNC  │ FLAGS │  SEQ  │SRC_ID │DST_ID │PAYLOAD│  TS   │ CRC16 │
│ 0xD3  │vvvvtttt│ BE u16│       │       │4 bytes│24-bit │CCITT  │
└───────┴───────┴───────┴───────┴───────┴───────┴───────┴───────┘
  1 B     1 B    2 B     2 B     2 B     4 B     3 B     2 B
```

| Field | Description |
|-------|-------------|
| SYNC | `0xD3` preamble — first validity gate |
| FLAGS | `[7:4]` version · `[3:0]` type (`0=DATA 1=ACK 2=BEACON 3=CTRL`) |
| SEQ | Big-endian rolling counter (replay detection) |
| SRC_ID / DST_ID | Node IDs · `0xFFFF` = broadcast |
| PAYLOAD | 4 bytes application data |
| TIMESTAMP | 24-bit big-endian µs offset, wraps ~16.7 s |
| CRC16 | CRC-CCITT poly `0x1021`, init `0xFFFF`, over bytes `[0..14]` |

Valid iff `sync == 0xD3` **and** `crc16_ccitt(frame[0..14]) == frame[15..16]`.

---

## The 8-Byte JackFrame (Audio-Band Links)

A stripped-down mini-frame for latency-critical point-to-point links over guitar cables or acoustic channels. No routing metadata — just payload and integrity check.

```
 0       1       2-5       6-7
┌───────┬───────┬──────────┬───────┐
│ SYNC  │  HDR  │ PAYLOAD  │ CRC16 │
│ 0xA5  │ssssttt│  4 bytes │ CCITT │
└───────┴───────┴──────────┴───────┘
  1 B     1 B      4 B       2 B
```

| Field | Description |
|-------|-------------|
| SYNC | `0xA5` — distinct from DCF `0xD3`, no ambiguity on mixed links |
| HDR | `[7:4]` seq nibble (0–15 rolling) · `[3:0]` type (`0=DATA 1=ACK 2=BEACON 3=CTRL`) |
| PAYLOAD | 4 bytes: `float32`, `uint32`, or `4×uint8` |
| CRC16 | CRC-CCITT over bytes `[0..5]` |

**Reference vector** — DATA, seq=0, payload=`DEADBEEF`:
```
Wire: A5 01 DE AD BE EF C2 3F    CRC pin: 0xC23F
```

Bridge to DCF: `jfToDCF` in `DCF.Transport.JackFrame` wraps the 4-byte payload into a full DCF DATA frame for forwarding into the wider network.

### Cross-Language Codec Parity

**Reference test vector** — version=1, DATA, seq=1, src=1, dst=`0xFFFF`, payload=`DEADBEEF`, ts=0:

```
Bytes [0..14]:  D3 10 00 01  00 01 FF FF  DE AD BE EF  00 00 00
CRC   [15..16]: 42 DD
Full frame:     D3 10 00 01 00 01 FF FF DE AD BE EF 00 00 00 42 DD
```

The CRC pin `0x42DD` is asserted in every language's test suite. Divergence is a wire-breaking change.

| Language | File | Encode | Decode | Validate | CRC pin test |
|----------|------|--------|--------|----------|--------------|
| C | `transport/dcf_frame.h` | `dcf_frame_encode()` | `dcf_frame_decode()` | `dcf_frame_valid()` | `src/test_conformance.cpp` |
| Haskell | `DCF/Transport/Frame.hs` | `encodeFrame` | `decodeFrame` | `validateFrame` | `test/ConformanceSpec.hs` |
| Rust | `dcf/rust/src/frame.rs` | `Frame::encode()` | `Frame::decode()` | `Frame::is_valid()` | `#[test] crc_cross_language_pin` |
| Common Lisp | `hydramesh.lisp` | `encode-dcf-frame` | `decode-dcf-frame` | `valid-dcf-frame-p` | `dcf-frame-crc-test` |
| Go | `dcf/go/frame.go` | `Frame.Encode()` | `Decode()` | `IsValid()` | `TestCRCPin` |
| Python (GR) | `gr-demod/python/demod/` | `encode_frame()` | `decode_frame()` | inline | `qa_dcf_frame.py::TestCRCPin` |

---

## DCF Wire Protocol (TLV Envelope)

The C SDK and Python SDK use a compact TLV envelope over any transport plugin:

```
magic(1) version(1) type(1) seq(4 BE) timestamp_us(8 BE)
sender_len(1) sender(N) recipient_len(1) recipient(M)
group_id_len(1) group_id(G) data_len(4 BE) data(D)
msg_id_len(1) msg_id(I)
```

`magic = 0xDC`, `version = 0x01`. All multi-byte integers big-endian.

---

## Quickstart

### Haskell TX

```bash
cd haskell && nix develop
faust-all-mod     # compile all modulator DSP files → *_gen.rs (once per DSP change)
cabal-build && cabal-test
cabal run demod-sdr-hs -- --driver rtlsdr --freq 433.92e6 --rate 2e6 --gain 20
```

### Haskell RX

```bash
cd haskell && nix develop
cabal run demod-rx-hs -- --driver rtlsdr --freq 433.92e6 --rate 2e6 --gain 40
```

### C++ standalone

```bash
nix develop
cmake -B build -G Ninja
faust-all-mod && faust-all-demod && faust-all-audio   # compile all DSP → *_gen.rs
cmake --build build
ctest --test-dir build -V                    # conformance + frame tests
./build/demod-sdr
```

### Rust

```bash
cd dcf/rust && cargo test
```

### Go

```bash
cd dcf/go && go test -v
```

### HydraMesh (Lisp)

```bash
sbcl --load hydramesh.lisp --eval '(run-tests)'
```

### GNU Radio OOT

```bash
cd gr-demod
cmake -B build && cmake --build build && sudo cmake --install build
python3 -m pytest python/demod/qa_dcf_frame.py -v   # no hardware required
```

### SDR Transport Plugin

```bash
cd dcf/c_sdk/plugins
gcc -shared -fPIC -o libdcf_sdr_transport.so sdr_transport.c -lcjson -lpthread
# config.json: "transport": "libdcf_sdr_transport.so"
# demod-sdr-hs must be running to open the named pipes
```

### Acoustic FSK Modem (speaker + microphone)

Transmits JackFrame data as audible two-tone FSK. Sounds like a dial-up modem. Works between any two machines with a speaker and a microphone within ~50 cm.

```bash
cd haskell && nix develop

# 1. Compile the acoustic DSP (once per change):
faust-all-audio   # compiles acoustic_fsk_mod.dsp + acoustic_fsk_demod.dsp → *_gen.rs

# 2. Build:
cabal build acoustic-hello-tx acoustic-hello-rx

# 3. Machine B — start listener first:
cabal run acoustic-hello-rx

# 4. Machine A — interactive TX prompt:
cabal run acoustic-hello-tx
# send> HELLO
# send> DeMoD LLC
# send>        ← empty line to quit
```

**Parameters:** mark=2000 Hz, space=3000 Hz, 1200 baud, 48 kHz SR, sps=40, frame=53.3 ms.
See `acoustic_fsk_guide.docx` for full tuning, troubleshooting, and JACK/PipeWire routing.

**Single-machine loopback test** (no second machine needed):
```bash
pactl load-module module-null-sink sink_name=loopback
pactl load-module module-loopback source=loopback.monitor
# run TX and RX in separate terminals — they talk through PipeWire
```

---

## Faust DSP Parameters

| Path | Default | Range | Notes |
|------|---------|-------|-------|
| `/modulator/carrier_freq` | `0.1` | `0.001–0.5` | Normalized to SR/2 |
| `/modulator/symbol_rate` | `9600` | `300–115200` | Baud |
| `/modulator/mod_index` | `π` | `0–2π` | `π` = BPSK |
| `/modulator/rrc_rolloff` | `0.35` | — | Label only; FIR is compiled in (α=0.35) |
| `/modulator/output_gain` | `0.8` | `0–1.0` | Headroom |
| `/bpsk_demod/symbol_rate` | `9600` | `300–115200` | Must match TX |
| `/bpsk_demod/loop_bw` | `0.02` | `0.001–0.1` | Costas loop BW |
| `/bpsk_demod/output_gain` | `1.0` | `0–4.0` | Soft symbol scale |
| `/qpsk_demod/symbol_rate` | `28800` | `300–115200` | Must match TX |
| `/qpsk_demod/loop_bw` | `0.02` | `0.001–0.1` | 4-phase Costas loop BW |
| `/qpsk_demod/output_gain` | `1.0` | `0–4.0` | Soft symbol scale |
| `/jack_demod/loop_bw` | `0.03` | `0.001–0.1` | Guitar-cable Costas BW |
| `/acoustic_fsk_demod/agc_tc` | `0.05` | — | AGC time constant (50 ms) |

---

## DCF Plugin Interface

### Transport plugin (C)

```c
ITransport* dcf_transport_create(void) {
    ITransport* tp    = calloc(1, sizeof(ITransport));
    tp->name          = "my-transport";
    tp->version_major = 1;    // must be >= 1.1
    tp->version_minor = 1;
    tp->setup         = my_setup;
    tp->send          = my_send;
    tp->receive       = my_receive;
    tp->health_check  = my_health_check;
    tp->destroy       = my_destroy;
    return tp;
}
```

`dlopen()` handles are stored and `dlclose()`'d at `dcf_plugin_manager_destroy()`. Transport load failure is fatal; hardware plugin failures are non-fatal.

---

## DCF Config Reference

```jsonc
{
  "node_id":    "node-alpha",
  "mode":       "p2p",
  "listen_addr": "0.0.0.0:9000",
  "peers":      ["10.0.0.2:9000"],
  "plugins": {
    "transport": "libdcf_sdr_transport.so",
    "hardware":  { "i2c_sensor": "libi2c_sensor.so" }
  },
  "transport_params": {
    "tx_pipe":          "/tmp/dcf_sdr_tx",
    "rx_pipe":          "/tmp/dcf_sdr_rx",
    "frame_timeout_ms": 500
  },
  "timeout_ms": 5000,
  "retries":    3,
  "redundancy": { "enabled": true, "reroute_timeout_s": 10 }
}
```

---

## Distribution

Pre-built packages for all four binaries (`acoustic-hello-tx`, `acoustic-hello-rx`, `demod-sdr-hs`, `demod-rx-hs`).

### Docker

```bash
nix build .#docker
docker load < result
# Acoustic TX — mount host PipeWire/JACK socket:
docker run --rm -it \
  -v /run/user/1000/pipewire-0:/run/user/1000/pipewire-0 \
  -v /run/user/1000/jack:/run/user/1000/jack \
  demod-faust-sdr:latest acoustic-hello-tx
# SDR TX — pass through USB device:
docker run --rm -it --device /dev/bus/usb \
  demod-faust-sdr:latest demod-sdr-hs
```

Layered image — runtime libs and SDR plugins in low layers, binaries on top. Only the top layer rebuilds on code changes.

### AppImage (any Linux ≥ kernel 4.x, no install required)

```bash
nix build .#appimage-hello-tx && chmod +x result && ./result
nix build .#appimage-hello-rx && chmod +x result && ./result
nix build .#appimage-sdr-tx   && chmod +x result && ./result
nix build .#appimage-sdr-rx   && chmod +x result && ./result
```

Bundles the binary and all `.so` dependencies via patchelf rpath. No Nix, no Docker, no package manager. JACK/PipeWire must be running on the host.

### Tarball (unpack and run anywhere)

```bash
nix build .#tarball
tar xf result/demod-faust-sdr.tar.gz
./demod-faust-sdr/run acoustic-hello-tx
./demod-faust-sdr/run demod-sdr-hs
```

Contains `bin/`, a flat `lib/` with the full runtime closure, and a `run` launcher that sets `LD_LIBRARY_PATH` and `SOAPY_SDR_PLUGIN_PATH` automatically relative to the unpacked directory.

### Nix bundle (self-extracting, no Nix needed on target)

```bash
nix build .#bundle-hello-tx   # → single self-extracting shell script
nix build .#bundle-hello-rx
# or, using the nix-appimage bundler:
nix bundle --bundler github:ralismark/nix-appimage .#hello-tx
```

Packs the entire Nix store closure into a self-extracting archive. Extracts to `/tmp` on first run. Largest format (~500 MB unpacked) but requires nothing on the target machine.

---

## Nix Dev Shells

| Shell | Command | Includes |
|-------|---------|----------|
| Full | `cd haskell && nix develop` | GHC 9.10, HLS, cabal, Faust 2.83, SoapySDR, GNURadio, inspectrum, JACK2 |
| Headless | `cd haskell && nix develop .#headless` | GHC, cabal, Faust, SoapySDR, JACK2 |
| Modem dev | `cd haskell && nix develop .#modem-dev` | Headless + liquid-dsp, codec2, minimodem, sox, baudline, PipeWire |
| Embedded | `cd haskell && nix develop .#embedded` | Rust, ARM cross-compiler, openocd, probe-rs, cargo-embed |

---

## SDK Status

| Language | Frame Codec | Plugin Manager | Transport | Redundancy | Status |
|----------|-------------|---------------|-----------|------------|--------|
| C | ✓ DCF + JackFrame | ✓ dlopen/linked-list | ✓ UDP + SDR | ✓ Dijkstra | Complete |
| Haskell | ✓ DCF + JackFrame | — | ✓ SoapySDR TX/RX + Acoustic FSK | — | TX + RX + Acoustic complete |
| Rust | ✓ no_std | ✓ traits/HashMap | stub | — | Frame + core complete |
| Common Lisp | ✓ | ✓ CLOS/hash-table | ✓ UDP (HydraMesh) | — | Complete |
| Python | — | ✓ ABC/importlib | ✓ TLV encode/decode | — | Core complete |
| Go | ✓ | — | — | — | Frame codec complete |
| GNU Radio OOT | ✓ (Python) | — | ✓ byte stream | — | Sink + source complete |

---

## Faust 2.73+ Migration Notes

All DSP files were migrated from pre-2.73 Faust syntax to Faust 2.83 (`nixpkgs` binary):

| Old syntax | Replacement | Files affected |
|------------|-------------|----------------|
| `let { 'x = ...; } in expr` | `with {}` local bindings | all demod files |
| `letrec { 'x = ...; } in expr` | `pll_core_*` function + `~ (_, _)` | `bpsk_demod`, `qpsk_demod`, `jack_demod` |
| `'x = expr` (prime recursive binding) | `pll_core_*` state-machine function | Costas loop files |
| `\(a, b) -> expr` (lambda) | named `with {}` local function | `acoustic_fsk_demod` |
| Trailing comma in `waveform{}` | removed | `qpsk_mod`, all demod files with RRC tables |
| `-lang c++` backend | `-lang rust` | `flake.nix` (C++ backend not compiled in nixpkgs binary) |

Costas loop PLL architecture: all three Costas loop implementations (`bpsk_demod`, `qpsk_demod`, `jack_demod`) were rewritten using a `pll_core_*(i_s, q_s, ph, fi)` state-transition function whose `with{}` bindings only reference its own arguments (no mutual cross-references), combined with `~ (_, _)` to feed `(ph_new, fi_new)` back as `(ph, fi)` with one sample of delay. This avoids the definitional cycle that Faust's term-rewriting evaluator cannot resolve when `with{}` bindings reference each other.

---

## Known Limitations

- **GMSK/FSK/ASK filters** — GMSK uses 3× cascaded IIR as a Gaussian approximation; FSK/ASK use IIR lowpass matched filters. These are not matched filters in the strict ISI-minimisation sense. Adequate for bring-up; a proper Gaussian FIR (erf-based coefficients) would improve GMSK BER by ~1 dB.
- **Costas loop gains** — computed from `loop_bw` via a continuous-time 2nd-order approximation. Empirical tuning against real IQ captures is required for production lock performance.
- **Frame sync** — `DCF.Demodulator.findFrame` uses a naive byte-aligned sliding window. A matched filter correlator in the soft domain would provide 3–5 dB better sensitivity in noise.
- **QPSK RX** — `DCF.Modem.QPSK` provides the symbol de-mapper; the full QPSK demodulator pipeline (`qpsk_demod.dsp` + Haskell soft-symbol slicing + Gray decoding) is not yet wired in `runDemodulator` (only BPSK is currently the default RX scheme).
- **SDR transport fragmentation** — `sdr_transport.c` carries only 4 bytes of application data per DCF frame. Fragmentation for longer messages via `FCtrl` frames is not yet implemented.

---

## Roadmap

- [x] Docker image (`nix build .#docker`) — layered, `docker load`-ready
- [x] AppImage packages for all four binaries (`nix build .#appimage-*`)
- [x] Portable tarball with bundled libs and auto-launching `run` script (`nix build .#tarball`)
- [x] Nix bundle / self-extracting archive (`nix build .#bundle-*`)
- [x] Clean build — zero GHC warnings across all 6 Haskell source files
- [x] `jack-0.7.2.2` builds in dev shell (jack2 C headers added to all devShells)
- [ ] Wire QPSK, GMSK, FSK, ASK into `runDemodulator` as selectable schemes
- [ ] `FCtrl` fragmentation in `sdr_transport.c` for messages > 4 bytes
- [ ] Soft-domain correlator for frame sync (better Eb/N0 sensitivity)
- [ ] AUTO mode master node implementation in DCF
- [ ] Rust transport plugin (`libdcf_udp_transport.so`)
- [ ] GRC `.yml` block descriptor files for `dcf_frame_sink` and `dcf_frame_source`
- [ ] Node.js frame codec
- [ ] Wire `writeAudio` / `readAudio` stubs in `AcousticHelloTx` / `AcousticHelloRx` to JACK2 or PipeWire Simple API
- [ ] `processChunked` stubs in `AcousticFSK.hs` wired to `DCF.Modulator.processChunk`
- [ ] Jitter histogram demo (acoustic FSK interval log vs UDP ping comparison)

---

## License

lGPL-3.0. See `LICENSE`.

DCF excludes cryptographic primitives to comply with U.S. EAR and ITAR. Users adding encryption (e.g. TLS via a transport plugin) are responsible for their own export compliance. DeMoD LLC disclaims liability for non-compliant modifications.

---

*DeMoD LLC — Design ≠ Marketing*
