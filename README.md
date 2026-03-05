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
├── dsp/
│   └── modulator.dsp            # Faust DSP: autonomous BPSK-PM modulator
├── transport/
│   ├── demod_frame.h            # Legacy packed-struct C header (retained for compatibility)
│   └── dcf_frame.h              # Canonical C99 frame codec (encode/decode, no deps)
├── src/
│   └── test_frame.cpp           # C++ frame unit tests
├── captures/                    # IQ captures (.cf32) for inspectrum
│
├── haskell/                     # Haskell integration layer
│   ├── flake.nix                # Nix dev shell (GHC + full stack)
│   ├── dcf-faust-sdr.cabal      # Cabal package
│   ├── cbits/
│   │   ├── faust_bridge.h/cpp   # C wrapper for Faust C++ mydsp class
│   │   └── soapy_bridge.h/cpp   # C wrapper for SoapySDR C++ API
│   ├── src/
│   │   ├── DCF/Faust/FFI.hs     # Raw foreign imports (faust_bridge)
│   │   ├── DCF/Faust/DSP.hs     # Safe bracket-managed DSP handle
│   │   ├── DCF/SDR/SoapyFFI.hs  # Raw foreign imports (soapy_bridge)
│   │   ├── DCF/SDR/Device.hs    # Safe bracket-managed SDR device
│   │   ├── DCF/Transport/Frame.hs   # 17-byte frame type + Binary + CRC
│   │   ├── DCF/Transport/Symbol.hs  # BPSK encoder: frame → Vector Float
│   │   └── DCF/Modulator.hs     # Top-level TX pipeline
│   ├── app/
│   │   └── Main.hs              # CLI entry point (demod-sdr-hs)
│   ├── test/
│   │   └── FrameSpec.hs         # hspec + QuickCheck frame tests
│   └── dsp/
│       └── modulator_hs.dsp     # Faust DSP with 1-input symbol stream
│
├── lisp/
│   └── hydramesh.lisp           # HydraMesh v2.2.0: UDP gaming/audio SDK + DCF frame adapter
│
└── dcf/                         # DeMoD Communications Framework v5.0.0
    ├── messages.proto            # Protobuf wire format
    ├── services.proto            # gRPC service definitions
    ├── config.schema.json        # JSON Schema for config validation
    ├── config.json.example
    ├── c_sdk/                   # C SDK (most complete)
    │   ├── include/dcf_sdk/
    │   │   ├── dcf_error.h          # Error codes + dcf_strerror()
    │   │   ├── dcf_plugin_manager.h # ITransport, IHardwarePlugin, manager API
    │   │   ├── dcf_client.h         # Client API
    │   │   └── dcf_redundancy.h     # P2P redundancy API
    │   ├── src/dcf_sdk/
    │   │   ├── dcf_plugin_manager.c # dlopen loader, version gate, linked-list hw registry
    │   │   ├── dcf_client.c         # pthread recv loop, send + routing
    │   │   └── dcf_redundancy.c     # Dijkstra RTT routing, cluster grouping, self-heal
    │   ├── plugins/
    │   │   └── custom_transport.c   # Reference UDP transport plugin
    │   ├── tests/
    │   └── CMakeLists.txt
    ├── rust/src/
    │   ├── lib.rs               # Rust SDK (traits, Arc<Mutex>, serde)
    │   └── frame.rs             # Rust frame codec (encode/decode, no_std compatible)
    ├── lisp/src/d-lisp.lisp     # Common Lisp SDK (CLOS, hash-table registry)
    └── python/dcf/plugin_manager.py  # Python SDK (ABC, importlib, threading)
```

---

## Architecture Overview

### Faust-SDR Pipeline

The RF pipeline transforms application data into modulated IQ samples in three stages, each owned by a different language:

```
┌─────────────────────────────────────────────────────────────────┐
│  Haskell                                                         │
│                                                                  │
│  DeModFrame record  ──encodeFrame──►  ByteString (17 bytes)     │
│        │                                      │                  │
│  (type-safe,                      encodeBpsk (BPSK NRZ-L)       │
│   CRC at seal)                               │                   │
│                                    Vector Float (symbol stream)  │
│                                              │                   │
│                          ┌───── FFI ─────────┘                  │
└──────────────────────────┼──────────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────────┐
│  Faust (via faust_bridge.cpp C wrapper)                          │
│                                                                  │
│  inputs[0]: symbol stream                                        │
│       │                                                          │
│  RRC pulse shaping filter                                        │
│       │                                                          │
│  Phase modulation onto NCO carrier (I/Q split)                  │
│       │                                                          │
│  outputs[0] = I,  outputs[1] = Q                                │
│                                                                  │
│                          ┌───── FFI ─────────┐                  │
└──────────────────────────┼──────────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────────┐
│  SoapySDR (via soapy_bridge.cpp C wrapper)                       │
│                                                                  │
│  Interleaved CF32 [I0,Q0,I1,Q1,...] → writeStream → RF hardware │
└─────────────────────────────────────────────────────────────────┘
```

**Why C bridges?** Haskell's FFI calls C only. Faust generates C++ (`mydsp` class). SoapySDR is a C++ API. `cbits/faust_bridge.cpp` and `cbits/soapy_bridge.cpp` each wrap a C++ API in `extern "C"` flat functions. Cabal compiles them as `c-sources` and links `stdc++` automatically — no separate CMake step.

### DCF Framework

DCF is a transport-agnostic messaging framework. Every transport and hardware interaction is behind a plugin interface, loaded at runtime from shared libraries declared in `config.json`. The 17-byte frame is DCF's native transport unit when the Faust-SDR system is used as a DCF transport plugin.

```
┌────────────────────────────────────────────────────┐
│  Application                                        │
│         │  dcf_client_send(client, &msg)            │
│         ▼                                           │
│  DCF Client  ──► Redundancy (Dijkstra routing)      │
│         │                                           │
│  Plugin Manager                                     │
│    ├── ITransport (singleton)                       │
│    │     send() / receive() / health_check()        │
│    └── IHardwarePlugin[] (keyed by string ID)       │
│          execute("read") → result bytes             │
└────────────────────────────────────────────────────┘
          │
   Transport plugin (.so)
   e.g. libdcf_udp_transport.so
        or a future libdcf_sdr_transport.so
```

---

## The 17-Byte Transport Frame

The frame is the single shared contract between all language implementations. Every codec must produce and accept identical bytes for identical logical frames.

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
| SYNC | `0xD3` fixed preamble — first validity gate |
| FLAGS | `[7:4]` version (4-bit) · `[3:0]` type (`0=DATA 1=ACK 2=BEACON 3=CTRL`) |
| SEQ | Big-endian rolling sequence counter (replay detection) |
| SRC_ID / DST_ID | Node identifiers · `0xFFFF` = broadcast |
| PAYLOAD | 4 bytes of application data |
| TIMESTAMP | 24-bit big-endian µs offset, wraps ~16.7 s |
| CRC16 | CRC-CCITT (poly `0x1021`, init `0xFFFF`) over bytes `[0..14]` |

A frame is valid iff `sync == 0xD3` **and** `crc16_ccitt(frame[0..14]) == frame[15..16]`.

### Cross-Language Codec Parity

All four implementations produce identical bytes for identical inputs and are tested against a shared reference vector.

| Language | File | Encode | Decode | Validate | Tests |
|----------|------|--------|--------|----------|-------|
| C (C99, no deps) | `transport/dcf_frame.h` | `dcf_frame_encode()` | `dcf_frame_decode()` | `dcf_frame_valid()` | `src/test_frame.cpp` |
| Haskell | `DCF/Transport/Frame.hs` | `encodeFrame` | `decodeFrame` | `validateFrame` | `test/FrameSpec.hs` |
| Rust (no\_std) | `dcf/rust/src/frame.rs` | `Frame::encode()` | `Frame::decode()` | `Frame::is_valid()` | inline `#[cfg(test)]` |
| Common Lisp | `lisp/hydramesh.lisp` | `encode-dcf-frame` | `decode-dcf-frame` | `valid-dcf-frame-p` | `fiveam` suite |

**Reference test vector** — version=1, DATA, seq=1, src=1, dst=`0xFFFF`, payload=`DEADBEEF`, ts=0:

```
Bytes [0..14]:  D3 10 00 01  00 01 FF FF  DE AD BE EF  00 00 00
CRC   [15..16]: 42 DD
Full frame:     D3 10 00 01 00 01 FF FF DE AD BE EF 00 00 00 42 DD
```

Every codec must produce exactly this byte sequence for the above input and must accept it as valid on decode. The CRC value `0x42DD` is pinned in each language's test suite — any deviation indicates the `crc16_ccitt` algorithm has diverged from the others, which is a wire-breaking change.

---

## Frame Codec: C

`transport/dcf_frame.h` is a single-header, zero-dependency C99 implementation. Drop it into any C or C++ project with a single `#include`. Works on any platform including bare-metal embedded (RP2040, STM32, RISC-V) — no stdlib required beyond `<stdint.h>`, `<stdbool.h>`, and `<string.h>`.

All multi-byte fields are explicitly serialized big-endian by shifting bytes — no struct casting, no `#pragma pack` in the encode/decode path, no host byte-order dependency.

```c
#include "transport/dcf_frame.h"

// ── Build and encode ──────────────────────────────────────────────────────────

dcf_frame_t f;
dcf_frame_init(&f,
    /*version*/  1,
    /*type*/     DCF_TYPE_DATA,
    /*seq*/      42,
    /*src_id*/   0x0001,
    /*dst_id*/   DCF_BROADCAST);   // 0xFFFF

f.payload[0] = 0xDE; f.payload[1] = 0xAD;
f.payload[2] = 0xBE; f.payload[3] = 0xEF;
f.timestamp_us = 0x001234;

uint8_t wire[DCF_FRAME_SIZE];
dcf_frame_encode(&f, wire);        // writes 17 bytes; CRC appended automatically

// ── Parse received bytes ──────────────────────────────────────────────────────

dcf_frame_t out;
if (dcf_frame_decode(wire, &out)) {
    // out.version, out.type, out.seq, out.src_id, out.dst_id,
    // out.payload[4], out.timestamp_us are all populated
} else {
    // bad sync byte or CRC mismatch — discard
}

// ── Non-destructive hot-path validation ───────────────────────────────────────

if (dcf_frame_valid(wire)) {
    // structurally sound; decode only if you need the fields
}
```

`demod_frame.h` (the earlier packed-struct header) is retained for backward compatibility with existing code. New code should use `dcf_frame.h`.

---

## Frame Codec: Rust

`dcf/rust/src/frame.rs` is a `no_std`-compatible Rust module. `Frame::encode()` returns a stack-allocated `[u8; 17]` — no heap allocation anywhere in the encode or decode path.

```rust
use dcf::frame::{Frame, FrameType, BROADCAST};

// ── Build and encode ──────────────────────────────────────────────────────────

let frame = Frame::new(
    1,                          // version
    FrameType::Data,
    42,                         // seq
    0x0001,                     // src_id
    BROADCAST,                  // dst_id — 0xFFFF
    [0xDE, 0xAD, 0xBE, 0xEF],  // payload
    0x001234,                   // timestamp_us (24-bit, stored as u32)
);

let wire: [u8; 17] = frame.encode();   // CRC appended automatically; stack allocated

// ── Decode ────────────────────────────────────────────────────────────────────

match Frame::decode(&wire) {
    Ok(f)  => { /* f.version, f.frame_type, f.seq, f.src_id, f.dst_id,
                   f.payload, f.timestamp_us */ }
    Err(e) => { /* FrameError::BadSync | BadCrc | BadLength | UnknownType */ }
}

// ── Validate without decoding (cheap, no allocation) ─────────────────────────

if Frame::is_valid(&wire) {
    // sync and CRC are both good
}
```

Add `pub mod frame;` to `dcf/rust/src/lib.rs` to expose it. No Cargo dependencies are added — `frame.rs` uses only `core` primitives.

For `no_std` embedded targets:

```rust
// lib.rs
#![no_std]
pub mod frame;
// Frame::encode, Frame::decode, Frame::is_valid all work without std
```

### Running Rust frame tests

```bash
cd dcf/rust
cargo test frame        # frame codec tests only
cargo test              # full Rust SDK suite
```

The 15 tests are fully deterministic and require no network or hardware. `crc_cross_language_pin` is the most important: it asserts that `crc16_ccitt` over the reference body bytes produces exactly `0x42DD`. A failure there means the Rust CRC has diverged from C/Haskell/Lisp — a wire-breaking change that requires fixing before any cross-language interop will work.

---

## Frame Codec: Haskell

`DCF.Transport.Frame` in the Haskell package is the original reference implementation.

```haskell
import DCF.Transport.Frame

let frame = DeModFrame
      { frameVersion   = 1
      , frameType      = FData
      , frameSeq       = 42
      , frameSrcId     = 0x0001
      , frameDstId     = broadcast          -- 0xFFFF
      , framePayload   = (0xDE, 0xAD, 0xBE, 0xEF)
      , frameTimestamp = 0x001234
      }

-- sealFrame computes and attaches the CRC
let sealed = sealFrame frame

-- encodeFrame produces a strict ByteString of exactly 17 bytes
let wire = encodeFrame sealed

-- decodeFrame returns Nothing on bad sync or CRC mismatch
case decodeFrame wire of
  Just f  -> -- f :: DeModFrame
  Nothing -> -- corrupt or truncated
```

Tests in `haskell/test/FrameSpec.hs` cover all frame types, all version nibbles, sequence extremes, broadcast destination, timestamp boundaries, and a QuickCheck property that any single-byte corruption in body positions 1–14 fails CRC.

---

## Frame Codec: Common Lisp (HydraMesh)

`lisp/hydramesh.lisp` contains the HydraMesh v2.2.0 UDP gaming/audio SDK and a DCF frame adapter layer that bridges HydraMesh `proto-message` objects to the canonical 17-byte wire format for handoff to the RF pipeline.

```lisp
;; Encode a proto-message as a 17-byte DeModFrame for the Haskell RF TX pipeline.
;; Write the returned vector to a named pipe or shared memory that demod-sdr-hs reads.
(let ((wire (dcf-to-rf-frame msg :src-id 1 :dst-id +dcf-broadcast+)))
  ...)  ; wire is (vector (unsigned-byte 8)) of exactly 17 bytes

;; Decode a 17-byte frame received from the Haskell RF RX side.
;; Returns a proto-message or NIL on corrupt input.
(let ((msg (dcf-from-rf-frame incoming-bytes)))
  (when msg ...))

;; Run all fiveam tests including the four DCF frame codec tests
(run-tests)
```

The `crc16-ccitt` function uses the identical algorithm and is pinned to the same `0x42DD` reference value. The `dcf-frame-roundtrip-test`, `dcf-frame-corruption-test`, `dcf-frame-valid-predicate-test`, and `dcf-frame-crc-test` tests in `hydramesh-suite` cover the same cases as the C and Rust suites.

---

## Quickstart

### Prerequisites

- NixOS or any Linux with `nix` flakes enabled
- SDR hardware: RTL-SDR / HackRF / LimeSDR / PlutoSDR / USRP (for TX)

### Haskell pipeline (recommended entry point)

```bash
cd haskell
nix develop          # enters the GHC + Faust + SoapySDR shell

# 1. Compile the Faust DSP → C++ (run once, or when modulator_hs.dsp changes)
faust-build
# → writes haskell/build/modulator_gen.cpp

# 2. Build the Haskell library + executable
cabal-build

# 3. Run tests (no hardware required)
cabal-test
# → Frame round-trip, CRC corruption, QuickCheck properties, symbol encoder

# 4. Probe attached hardware
sdr-probe

# 5. TX beacon stream
cabal run demod-sdr-hs -- --driver rtlsdr --freq 433.92e6 --rate 2e6 --gain 20
```

### C++ standalone path

```bash
nix develop          # root flake

cmake -B build -G Ninja
cmake --build build --target faust-compile   # compile modulator.dsp → C++
cmake --build build                          # build demod-sdr + test-frame
ctest --test-dir build -V                    # run C++ frame tests
./build/demod-sdr                            # TX (autonomous test tone)
```

### Rust frame codec

```bash
cd dcf/rust
cargo test frame       # frame codec tests only — no hardware, no network
cargo test             # full Rust SDK suite
```

### DCF C SDK

```bash
cd dcf/c_sdk
cmake -B build -G Ninja && cmake --build build
ctest --test-dir build -V

cp ../config.json.example config.json
# Edit config.json: set node_id, mode, plugins.transport path

python3 -c "
import json, jsonschema
schema = json.load(open('../config.schema.json'))
cfg    = json.load(open('config.json'))
jsonschema.validate(cfg, schema)
print('config valid')
"
```

### HydraMesh (Lisp)

```bash
# SBCL + Quicklisp required
sbcl --load lisp/hydramesh.lisp --eval '(main "help")'

# Run all tests including frame codec suite
sbcl --load lisp/hydramesh.lisp --eval '(run-tests)'

# Start a node
sbcl --load lisp/hydramesh.lisp \
     --eval '(main "init" "config.json")' \
     --eval '(main "start")' \
     --eval '(main "add-peer" "node2" "192.168.1.100" "7777")'
```

---

## Faust DSP Parameters

Both `.dsp` files expose these controls via OSC path. In the Haskell pipeline, set them at runtime with `DCF.Faust.DSP.setParam`. In the C++ standalone path, use faustlive or OSC.

| Path | Default | Range | Notes |
|------|---------|-------|-------|
| `/modulator/carrier_freq` | `0.1` (normalized) | `0.001–0.5` | In `modulator_hs.dsp`: normalized to SR/2. Haskell sets this to `actual_hz / (rate/2)`. |
| `/modulator/symbol_rate` | `9600` | `300–115200` | Baud. Must match `SymbolConfig.symSymbolRate` on the Haskell side. |
| `/modulator/mod_index` | `π` | `0–2π` | Modulation depth. `π` = BPSK. |
| `/modulator/rrc_rolloff` | `0.35` | `0.1–0.9` | RRC filter rolloff α. |
| `/modulator/output_gain` | `0.8` | `0–1.0` | Headroom below DAC clipping. |

---

## DCF Plugin Interface

### Writing a transport plugin (C)

```c
// my_transport.c
// gcc -shared -fPIC -o libmy_transport.so my_transport.c -lcjson

#include "dcf_sdk/dcf_plugin_manager.h"

static DCFError my_setup(ITransport* self, const char* params, int timeout_ms) { ... }
static DCFError my_send(ITransport* self, const uint8_t* data, size_t len,
                         const char* target, int retries) { ... }
static DCFError my_receive(ITransport* self, uint8_t* buf, size_t buf_len,
                            size_t* out_len, int timeout_ms) { ... }
static DCFError my_health_check(ITransport* self, const char* target, double* rtt_ms) { ... }
static void     my_destroy(ITransport* self) { free(self); }

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

Register in `config.json`:
```json
{ "plugins": { "transport": "libmy_transport.so" } }
```

### Writing a transport plugin (Rust)

```rust
use dcf::{ITransport, DcfResult};
use dcf::frame::{Frame, FrameType};
use serde_json::Value;

struct MyTransport { seq: u16 }

impl ITransport for MyTransport {
    fn name(&self)          -> &str { "my-transport" }
    fn version_major(&self) -> u32  { 1 }
    fn version_minor(&self) -> u32  { 1 }

    fn setup(&mut self, _params: &Value, _timeout_ms: u64) -> DcfResult<()> { Ok(()) }

    fn send(&self, data: &[u8], _target: &str, _retries: u32) -> DcfResult<()> {
        let mut payload = [0u8; 4];
        payload.copy_from_slice(&data[..4.min(data.len())]);
        let wire = Frame::new(1, FrameType::Data, 0, 1, 0xFFFF, payload, 0).encode();
        // write wire[..] to socket / pipe
        Ok(())
    }

    fn receive(&self, _buf_size: usize, _timeout_ms: u64) -> DcfResult<(Vec<u8>, String)> {
        // read 17 bytes, decode:
        // let frame = Frame::decode(&raw_bytes)?;
        todo!()
    }

    fn health_check(&self, _target: &str) -> DcfResult<f64> { Ok(0.5) }
}
```

### Writing a hardware plugin (C)

```c
IHardwarePlugin* dcf_hardware_create(void) {
    IHardwarePlugin* hw = calloc(1, sizeof(IHardwarePlugin));
    hw->name          = "my-i2c-sensor";
    hw->id            = "i2c_sensor";   // key used in config.json
    hw->version_major = 1;
    hw->version_minor = 1;
    hw->setup         = my_hw_setup;
    hw->execute       = my_hw_execute;  // JSON command in, JSON result out
    hw->destroy       = my_hw_destroy;
    return hw;
}
```

DCF enforces version `>= 1.1` at load time. Transport load failure is fatal. Hardware plugin load failures are non-fatal (logged, system continues).

---

## DCF Config Reference

```jsonc
{
  "node_id":    "node-alpha",           // required
  "mode":       "p2p",                  // client | server | p2p | auto
  "listen_addr": "0.0.0.0:9000",
  "master_addr": "10.0.0.1:9000",       // required when mode=auto
  "peers":      ["10.0.0.2:9000"],

  "plugins": {
    "transport": "libdcf_udp_transport.so",   // required
    "hardware": {
      "i2c_sensor": "libi2c_sensor.so"
    }
  },
  "transport_params": { "bind_port": 9001 },
  "hardware_params":  { "i2c_sensor": { "bus": 1, "address": "0x48" } },

  "timeout_ms": 5000,
  "retries":    3,
  "rtt_group_threshold_ms": 50,
  "redundancy": {
    "enabled":           true,
    "reroute_timeout_s": 10
  }
}
```

Validate against `dcf/config.schema.json` before deployment. The schema enforces `master_addr` presence when `mode == "auto"`.

---

## Nix Dev Shells

| Shell | Command | Includes |
|-------|---------|----------|
| Haskell full | `cd haskell && nix develop` | GHC 9.6, HLS, cabal, Faust, SoapySDR, GNURadio, inspectrum |
| Haskell headless | `cd haskell && nix develop .#headless` | GHC, cabal, Faust, SoapySDR (no GUI tools) |
| C++ full | `nix develop` (root) | clang17, cmake, Faust, SoapySDR, GNURadio, inspectrum |
| C++ headless | `nix develop .#headless` | clang17, cmake, Faust, SoapySDR |

All shells set `CC=clang`, `CXX=clang++`, `FAUST_ARCH_PATH`, `PKG_CONFIG_PATH`, and scaffold the project directory layout on first entry.

---

## SDK Language Status

| Language | Location | Frame Codec | Plugin Manager | Transport | Redundancy | Status |
|----------|----------|-------------|---------------|-----------|------------|--------|
| C | `transport/dcf_frame.h` + `dcf/c_sdk/` | ✓ encode/decode/valid | ✓ dlopen/linked-list | ✓ UDP reference | ✓ Dijkstra | Complete |
| Haskell | `haskell/src/DCF/Transport/` | ✓ encodeFrame/decodeFrame | — | ✓ SoapySDR/Faust | — | Complete |
| Rust | `dcf/rust/src/frame.rs` + `lib.rs` | ✓ encode/decode/is_valid | ✓ traits/HashMap | stub | — | Frame + core complete |
| Common Lisp | `lisp/hydramesh.lisp` | ✓ encode-dcf-frame/decode-dcf-frame | ✓ CLOS/hash-table | ✓ UDP (HydraMesh) | — | Complete |
| Python | `dcf/python/` | — | ✓ ABC/importlib | stub | — | Core complete |
| C++ | `dcf/cpp/` | ✓ via `dcf_frame.h` | — | — | — | Header only |
| Go | `dcf/go/` | — | — | — | — | Stub |
| Node.js | `dcf/nodejs/` | — | — | — | — | Stub |

---

## Roadmap

**Frame codec**
- [ ] Conformance test harness — single script that feeds the reference vector to all SDK codecs and asserts identical output across C, Haskell, Rust, Lisp, Go, Node.js
- [ ] Go frame codec (`dcf/go/frame.go`)
- [ ] Node.js frame codec (`dcf/nodejs/frame.js`)
- [ ] CRC lookup-table variant in `dcf_frame.h` for embedded targets with limited flash

**Faust-SDR**
- [ ] Implement proper RRC FIR coefficient table (replace lowpass approximation)
- [ ] Add `arch/soapy-rx.cpp` + Haskell RX path (demodulate → validate CRC → decode frame)
- [ ] libfaust JIT hot-reload — swap modulation schemes at runtime without restart
- [ ] GNU Radio OOT block wrapper for integration testing

**DCF**
- [ ] Protobuf pack/unpack wired into C SDK client send/receive
- [ ] SDR transport plugin (`libdcf_sdr_transport.so`) bridging DCF → Faust-SDR pipeline
- [ ] Rust transport plugin (`libdcf_udp_transport.so` compiled from Rust)
- [ ] Complete C++, Go, Node.js SDK plugin managers
- [ ] AUTO mode master node implementation

---

## License

GPL-3.0. See `LICENSE`.

DCF excludes cryptographic primitives to comply with U.S. EAR and ITAR. Users adding encryption (e.g. TLS via a transport plugin) are responsible for their own export compliance. DeMoD LLC disclaims liability for non-compliant modifications.

---

*DeMoD LLC — Design ≠ Marketing*
