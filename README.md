# DeMoD — Faust-SDR & Communications Framework

**DeMoD LLC** · GPL-3.0 · [info@demod.ltd](mailto:info@demod.ltd)

Two codebases. One stack. A Haskell/Faust/SoapySDR RF modulation pipeline and a polyglot low-latency communications framework, sharing a custom 17-byte transport protocol at their boundary.

---

## Repository Layout

```
demod/
├── flake.nix                    # Root Nix dev shell (C++ path)
├── CMakeLists.txt               # C++ build (arch + transport tests)
├── arch/
│   └── soapy-sdr.cpp            # Faust architecture → SoapySDR TX (standalone C++ path)
├── dsp/                         # Faust DSP modem library
│   ├── modulator.dsp            # Autonomous BPSK-PM modulator (standalone C++ path)
│   ├── qpsk_mod.dsp             # QPSK modulator  (2 bps, 2-input I/Q)
│   ├── gmsk_mod.dsp             # GMSK modulator  (constant envelope, BT=0.3)
│   ├── fsk_mod.dsp              # FSK modulator   (h=0.5–2.0, CP via phasor NCO)
│   ├── ask_mod.dsp              # OOK/ASK modulator (raised-cosine shaping)
│   ├── bpsk_demod.dsp           # BPSK demodulator (Costas loop + matched filter)
│   ├── qpsk_demod.dsp           # QPSK demodulator (4-phase Costas loop)
│   ├── gmsk_demod.dsp           # GMSK demodulator (FM discriminator + limiter)
│   ├── fsk_demod.dsp            # FSK demodulator  (discriminator or dual-filter)
│   └── ask_demod.dsp            # OOK/ASK demodulator (envelope + adaptive slicer)
├── transport/
│   ├── demod_frame.h            # Legacy packed-struct header (compatibility)
│   └── dcf_frame.h              # Canonical C99 codec (zero deps, embedded-safe)
├── src/
│   └── test_frame.cpp           # C++ frame unit tests
│
├── haskell/                     # Haskell TX pipeline
│   ├── flake.nix
│   ├── dcf-faust-sdr.cabal
│   ├── cbits/
│   │   ├── faust_bridge.h/cpp   # extern "C" wrapper for Faust C++ mydsp
│   │   └── soapy_bridge.h/cpp   # extern "C" wrapper for SoapySDR C++ API
│   ├── src/
│   │   ├── DCF/Faust/FFI.hs     # Raw foreign imports (faust_bridge)
│   │   ├── DCF/Faust/DSP.hs     # Safe bracket-managed DSP handle
│   │   ├── DCF/SDR/SoapyFFI.hs  # Raw foreign imports (soapy_bridge)
│   │   ├── DCF/SDR/Device.hs    # Safe bracket-managed SDR device
│   │   ├── DCF/Transport/Frame.hs   # 17-byte frame type + Binary + CRC
│   │   ├── DCF/Transport/Symbol.hs  # BPSK encoder: frame → Vector Float
│   │   └── DCF/Modulator.hs     # Top-level TX pipeline
│   ├── app/Main.hs              # CLI entry point (demod-sdr-hs)
│   ├── test/FrameSpec.hs        # hspec + QuickCheck frame tests
│   └── dsp/modulator_hs.dsp    # Faust DSP: 1-input symbol stream → I/Q
│
├── hydramesh.lisp               # HydraMesh v2.2.0 UDP SDK + DCF frame adapter
│
└── dcf/                         # DeMoD Communications Framework v5.0.0
    ├── messages.proto
    ├── services.proto
    ├── config.schema.json
    ├── config.json.example
    ├── c_sdk/
    │   ├── include/dcf_sdk/
    │   │   ├── dcf_error.h
    │   │   ├── dcf_plugin_manager.h
    │   │   ├── dcf_client.h
    │   │   └── dcf_redundancy.h
    │   ├── src/dcf_sdk/
    │   │   ├── dcf_plugin_manager.c
    │   │   ├── dcf_client.c
    │   │   └── dcf_redundancy.c
    │   ├── plugins/custom_transport.c   # Reference UDP transport plugin
    │   └── CMakeLists.txt
    ├── rust/src/frame.rs
    ├── lisp/src/d-lisp.lisp
    └── python/dcf/plugin_manager.py
```

---

## Architecture Overview

### Faust-SDR Pipeline

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
│  Faust  (faust_bridge.cpp — extern "C" wrapper)                  │
│                                                                  │
│  inputs[0]: symbol stream (±1.0 NRZ)                            │
│  → Butterworth LPF (RRC approx)                                  │
│  → Phase modulation onto NCO carrier                             │
│  outputs[0] = I,  outputs[1] = Q                                │
│                          ┌───── FFI ───┐                        │
└──────────────────────────┼──────────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────────┐
│  SoapySDR  (soapy_bridge.cpp — extern "C" wrapper)               │
│                                                                  │
│  Interleaved CF32 → writeStream → RF hardware                   │
└─────────────────────────────────────────────────────────────────┘
```

### Modem DSP Library

Five modulator/demodulator pairs covering the DeMoD RF stack:

| Scheme | Modulator | Demodulator | Carrier Recovery |
|--------|-----------|-------------|-----------------|
| BPSK | `modulator_hs.dsp` | `bpsk_demod.dsp` | 2nd-order Costas loop (ζ=0.707) |
| QPSK | `qpsk_mod.dsp` | `qpsk_demod.dsp` | 4-phase Costas loop |
| GMSK | `gmsk_mod.dsp` | `gmsk_demod.dsp` | FM discriminator + limiter |
| FSK | `fsk_mod.dsp` | `fsk_demod.dsp` | Phase-deriv or dual-filter energy detector |
| OOK/ASK | `ask_mod.dsp` | `ask_demod.dsp` | Envelope detect + adaptive threshold |

All modulators accept ±1.0 NRZ symbol streams from Haskell and output 2-channel I/Q. All demodulators accept 2-channel I/Q baseband and output soft symbols for Haskell to slice.

### DCF Framework

```
┌────────────────────────────────────────────────────┐
│  Application                                        │
│         │  dcf_client_send(client, &msg)            │
│         ▼                                           │
│  DCF Client  ──► Redundancy (Dijkstra routing)      │
│         │                                           │
│  Plugin Manager                                     │
│    ├── ITransport  (singleton, dlopen'd .so)        │
│    └── IHardwarePlugin[]  (keyed by string ID)      │
└────────────────────────────────────────────────────┘
          │
   Transport plugin (.so)
   e.g. libdcf_udp_transport.so
```

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

### Cross-Language Codec Parity

| Language | File | Encode | Decode | Validate | Tests |
|----------|------|--------|--------|----------|-------|
| C | `transport/dcf_frame.h` | `dcf_frame_encode()` | `dcf_frame_decode()` | `dcf_frame_valid()` | `src/test_frame.cpp` |
| Haskell | `DCF/Transport/Frame.hs` | `encodeFrame` | `decodeFrame` | `validateFrame` | `test/FrameSpec.hs` |
| Rust | `dcf/rust/src/frame.rs` | `Frame::encode()` | `Frame::decode()` | `Frame::is_valid()` | `#[cfg(test)]` |
| Common Lisp | `hydramesh.lisp` | `encode-dcf-frame` | `decode-dcf-frame` | `valid-dcf-frame-p` | fiveam suite |

**Reference test vector** — version=1, DATA, seq=1, src=1, dst=`0xFFFF`, payload=`DEADBEEF`, ts=0:

```
Bytes [0..14]:  D3 10 00 01  00 01 FF FF  DE AD BE EF  00 00 00
CRC   [15..16]: 42 DD
Full frame:     D3 10 00 01 00 01 FF FF DE AD BE EF 00 00 00 42 DD
```

The CRC pin `0x42DD` is asserted in every language's test suite. Divergence is a wire-breaking change.

---

## DCF Wire Protocol (TLV Envelope)

The C SDK and Python SDK wrap DCF messages in a compact TLV envelope over any transport plugin. All SDK implementations produce and accept identical bytes.

```
magic(1) version(1) type(1) seq(4 BE) timestamp_us(8 BE)
sender_len(1) sender(N) recipient_len(1) recipient(M)
group_id_len(1) group_id(G) data_len(4 BE) data(D)
msg_id_len(1) msg_id(I)
```

- `magic = 0xDC`, `version = 0x01`
- All multi-byte integers: big-endian
- String length prefix: 1 byte (max 63 UTF-8 bytes per string)

---

## Quickstart

### Haskell pipeline (recommended)

```bash
cd haskell
nix develop

faust-build       # compile modulator_hs.dsp → C++ (once per DSP change)
cabal-build
cabal-test        # no hardware required

sdr-probe         # list attached hardware
cabal run demod-sdr-hs -- --driver rtlsdr --freq 433.92e6 --rate 2e6 --gain 20
```

### C++ standalone

```bash
nix develop
cmake -B build -G Ninja
cmake --build build --target faust-compile
cmake --build build
ctest --test-dir build -V
./build/demod-sdr
```

### Rust

```bash
cd dcf/rust
cargo test    # no hardware, no network
```

### DCF C SDK

```bash
cd dcf/c_sdk
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -V
```

### HydraMesh (Lisp)

```bash
sbcl --load hydramesh.lisp --eval '(run-tests)'
sbcl --load hydramesh.lisp --eval '(main "init" "config.json")' \
                            --eval '(main "start")'
```

---

## Faust DSP Parameters

Set via `DCF.Faust.DSP.setParam` (Haskell) or OSC (standalone).

| Path | Default | Range | Notes |
|------|---------|-------|-------|
| `/modulator/carrier_freq` | `0.1` | `0.001–0.5` | Normalized to SR/2 in `modulator_hs.dsp` |
| `/modulator/symbol_rate` | `9600` | `300–115200` | Baud |
| `/modulator/mod_index` | `π` | `0–2π` | `π` = BPSK |
| `/modulator/rrc_rolloff` | `0.35` | `0.1–0.9` | Rolloff α |
| `/modulator/output_gain` | `0.8` | `0–1.0` | Headroom below DAC clipping |

---

## DCF Plugin Interface

### Transport plugin (C)

```c
#include "dcf_sdk/dcf_plugin_manager.h"
// gcc -shared -fPIC -o libmy_transport.so my_transport.c -lcjson

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

The plugin manager stores the `dlopen()` handle and calls `dlclose()` at `dcf_plugin_manager_destroy()`.

### Hardware plugin (C)

```c
IHardwarePlugin* dcf_hardware_create(void) {
    IHardwarePlugin* hw = calloc(1, sizeof(IHardwarePlugin));
    hw->name          = "my-sensor";
    hw->id            = "i2c_sensor";   // key in config.json
    hw->version_major = 1;
    hw->version_minor = 1;
    hw->setup         = my_hw_setup;
    hw->execute       = my_hw_execute;
    hw->destroy       = my_hw_destroy;
    return hw;
}
```

Transport load failure is fatal. Hardware failures are non-fatal (logged, system continues).

---

## DCF Config Reference

```jsonc
{
  "node_id":    "node-alpha",
  "mode":       "p2p",              // client | server | p2p | auto
  "listen_addr": "0.0.0.0:9000",
  "master_addr": "10.0.0.1:9000",   // required when mode=auto
  "peers":      ["10.0.0.2:9000"],
  "plugins": {
    "transport": "libdcf_udp_transport.so",
    "hardware":  { "i2c_sensor": "libi2c_sensor.so" }
  },
  "transport_params": { "bind_port": 9001 },
  "hardware_params":  { "i2c_sensor": { "bus": 1, "address": "0x48" } },
  "timeout_ms": 5000,
  "retries":    3,
  "rtt_group_threshold_ms": 50,
  "redundancy": { "enabled": true, "reroute_timeout_s": 10, "max_hops": 8 }
}
```

Validate against `dcf/config.schema.json` before deployment.

---

## Nix Dev Shells

| Shell | Command | Includes |
|-------|---------|----------|
| Haskell full | `cd haskell && nix develop` | GHC 9.6, HLS, cabal, Faust, SoapySDR, GNURadio, inspectrum |
| Haskell headless | `cd haskell && nix develop .#headless` | GHC, cabal, Faust, SoapySDR |
| Modem dev | `cd haskell && nix develop .#modem-dev` | Headless + liquid-dsp, codec2, minimodem |
| C++ full | `nix develop` (root) | clang17, cmake, Faust, SoapySDR, GNURadio, inspectrum |
| C++ headless | `nix develop .#headless` | clang17, cmake, Faust, SoapySDR |
| Embedded | `nix develop .#embedded` | ARM cross-compiler, avr-gcc, openocd |

---

## SDK Status

| Language | Frame Codec | Plugin Manager | Transport | Redundancy | Status |
|----------|-------------|---------------|-----------|------------|--------|
| C | ✓ | ✓ dlopen/linked-list | ✓ UDP reference | ✓ Dijkstra | Complete |
| Haskell | ✓ | — | ✓ SoapySDR/Faust | — | TX pipeline complete |
| Rust | ✓ no_std | ✓ traits/HashMap | stub | — | Frame + core complete |
| Common Lisp | ✓ | ✓ CLOS/hash-table | ✓ UDP (HydraMesh) | — | Complete |
| Python | — | ✓ ABC/importlib | ✓ TLV encode/decode | — | Core complete |

---

## Known Limitations

- **RRC filter** — all DSP files use a 4th-order Butterworth lowpass as an approximation. Replace with a Kaiser-windowed FIR before any §15.249 certification submission.
- **Costas loop gains** — computed from `loop_bw` via a continuous-time 2nd-order approximation. Empirical tuning against real IQ captures is required before production deployment.
- **RX pipeline** — five demodulator DSP files exist but no `DCF.Demodulator` Haskell module or SoapySDR RX path yet.
- **CMakeLists.txt** — the 10 new DSP files are not yet added to the `faust-compile` custom target.

---

## Roadmap

- [ ] Haskell RX pipeline (`DCF.Demodulator`, SoapySDR RX path, soft-symbol slicer)
- [ ] Proper RRC FIR (erf-based, 63-tap Kaiser window) in all DSP files
- [ ] Conformance test harness asserting reference vector parity across all codecs
- [ ] SDR transport plugin (`libdcf_sdr_transport.so`) bridging DCF → Faust-SDR
- [ ] CMakeLists.txt: add all 10 new `.dsp` files to `faust-compile` target
- [ ] GNU Radio OOT block wrapper
- [ ] Go and Node.js frame codecs
- [ ] AUTO mode master node implementation

---

## License

LGPL-3.0. See `LICENSE`.

DCF excludes cryptographic primitives to comply with U.S. EAR and ITAR. Users adding encryption (e.g. TLS via a transport plugin) are responsible for their own export compliance. DeMoD LLC disclaims liability for non-compliant modifications.

---

*DeMoD LLC — Design ≠ Marketing*
