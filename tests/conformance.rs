// tests/conformance.rs — DeMoD cross-language codec conformance harness
// DeMoD LLC | GPL-3.0
//
// Asserts the canonical reference test vector against the Rust codec in
// src/frame.rs, including the CRC pin 0x42DD.
//
// This is the source-of-truth test.  The same vector is pinned in:
//   C        transport/dcf_frame.h   (test_conformance.cpp)
//   Haskell  haskell/test/FrameSpec.hs   (crcCrossLanguagePin)
//   Rust     src/frame.rs                (#[test] crc_cross_language_pin)  ← this file
//   Lisp     hydramesh.lisp              (dcf-frame-crc-test)
//
// If ANY language's test fails the pin, that codec has diverged from the
// others — a wire-breaking change that must be fixed before deployment.
//
// Run:
//   cargo test --test conformance

use demod::frame::{Frame, FrameType, BROADCAST, FRAME_SIZE, crc16_ccitt};

// ── Canonical reference vector ────────────────────────────────────────────────
//
// Input:
//   version=1, type=DATA(0), seq=1, src_id=1, dst_id=0xFFFF,
//   payload={0xDE,0xAD,0xBE,0xEF}, timestamp_us=0
//
// Expected wire bytes (17):
//   D3 10 00 01  00 01 FF FF  DE AD BE EF  00 00 00  42 DD
//
// CRC-CCITT over bytes [0..14] == 0x42DD  — pinned across all language codecs.

const REFERENCE_FRAME: [u8; FRAME_SIZE] = [
    0xD3, 0x10, 0x00, 0x01,   // sync, flags(ver=1,type=DATA), seq BE
    0x00, 0x01, 0xFF, 0xFF,   // src_id, dst_id (broadcast)
    0xDE, 0xAD, 0xBE, 0xEF,  // payload
    0x00, 0x00, 0x00,          // timestamp_us = 0 (24-bit BE)
    0x42, 0xDD,                // CRC16-CCITT pin
];

// ── Helpers ───────────────────────────────────────────────────────────────────

fn reference_decoded() -> Frame {
    Frame::new(1, FrameType::Data, 1, 1, BROADCAST, [0xDE, 0xAD, 0xBE, 0xEF], 0)
}

// ── CRC pin ───────────────────────────────────────────────────────────────────

/// The CRC-CCITT of the 15-byte frame body must be exactly 0x42DD.
/// This value is pinned across all language codecs; any divergence is a
/// wire-breaking change.
#[test]
fn crc_pin_valid_reference_frame() {
    assert!(
        Frame::is_valid(&REFERENCE_FRAME),
        "CRC pin 0x42DD: is_valid(reference_frame) must be true"
    );
}

#[test]
fn crc_pin_flipped_byte_invalid() {
    let mut bad = REFERENCE_FRAME;
    bad[15] ^= 0x01; // flip one CRC byte
    assert!(
        !Frame::is_valid(&bad),
        "flipped CRC byte must make is_valid() return false"
    );
}

// ── Encode matches reference vector byte-for-byte ────────────────────────────

#[test]
fn encode_matches_reference_vector() {
    let wire = reference_decoded().encode();
    assert_eq!(
        wire, REFERENCE_FRAME,
        "encode() output must match reference vector byte-for-byte"
    );
}

#[test]
fn encode_sync_byte()          { assert_eq!(reference_decoded().encode()[0],  0xD3); }
#[test]
fn encode_flags()              { assert_eq!(reference_decoded().encode()[1],  0x10, "ver=1, type=DATA"); }
#[test]
fn encode_seq_high()           { assert_eq!(reference_decoded().encode()[2],  0x00); }
#[test]
fn encode_seq_low()            { assert_eq!(reference_decoded().encode()[3],  0x01); }
#[test]
fn encode_src_id_high()        { assert_eq!(reference_decoded().encode()[4],  0x00); }
#[test]
fn encode_src_id_low()         { assert_eq!(reference_decoded().encode()[5],  0x01); }
#[test]
fn encode_dst_id_high()        { assert_eq!(reference_decoded().encode()[6],  0xFF); }
#[test]
fn encode_dst_id_low_broadcast(){ assert_eq!(reference_decoded().encode()[7], 0xFF); }
#[test]
fn encode_payload_0xde()       { assert_eq!(reference_decoded().encode()[8],  0xDE); }
#[test]
fn encode_payload_0xad()       { assert_eq!(reference_decoded().encode()[9],  0xAD); }
#[test]
fn encode_payload_0xbe()       { assert_eq!(reference_decoded().encode()[10], 0xBE); }
#[test]
fn encode_payload_0xef()       { assert_eq!(reference_decoded().encode()[11], 0xEF); }
#[test]
fn encode_timestamp_zero()     {
    let w = reference_decoded().encode();
    assert_eq!((w[12], w[13], w[14]), (0x00, 0x00, 0x00));
}
#[test]
fn encode_crc_high_pin()       { assert_eq!(reference_decoded().encode()[15], 0x42, "CRC high pin"); }
#[test]
fn encode_crc_low_pin()        { assert_eq!(reference_decoded().encode()[16], 0xDD, "CRC low pin"); }

// ── Decode reference vector ───────────────────────────────────────────────────

#[test]
fn decode_reference_ok() {
    assert!(Frame::decode(&REFERENCE_FRAME).is_ok());
}

#[test]
fn decode_reference_fields() {
    let f = Frame::decode(&REFERENCE_FRAME).unwrap();
    assert_eq!(f.version,      1);
    assert_eq!(f.frame_type,   FrameType::Data);
    assert_eq!(f.seq,          1);
    assert_eq!(f.src_id,       0x0001);
    assert_eq!(f.dst_id,       0xFFFF);
    assert_eq!(f.payload,      [0xDE, 0xAD, 0xBE, 0xEF]);
    assert_eq!(f.timestamp_us, 0);
}

// ── Roundtrip ─────────────────────────────────────────────────────────────────

#[test]
fn roundtrip_arbitrary_frame() {
    let orig = Frame::new(1, FrameType::Ack, 0xABCD, 0x0042, 0x0007, [0x11, 0x22, 0x33, 0x44], 0xA1B2C3);
    let wire = orig.encode();
    let back = Frame::decode(&wire).expect("roundtrip must succeed");

    assert_eq!(back.version,      orig.version);
    assert_eq!(back.frame_type,   orig.frame_type);
    assert_eq!(back.seq,          orig.seq);
    assert_eq!(back.src_id,       orig.src_id);
    assert_eq!(back.dst_id,       orig.dst_id);
    assert_eq!(back.payload,      orig.payload);
    // timestamp is stored as 24-bit only
    assert_eq!(back.timestamp_us, orig.timestamp_us & 0x00FF_FFFF);
}

// ── Corruption detection ──────────────────────────────────────────────────────

/// Every single-byte flip in the body (bytes 1–14) must be detected by the CRC.
#[test]
fn every_body_byte_flip_detected() {
    let orig = Frame::new(1, FrameType::Data, 0, 1, BROADCAST, [0xCA, 0xFE, 0xBA, 0xBE], 0);
    let wire = orig.encode();

    for i in 1..15 {
        let mut bad = wire;
        bad[i] ^= 0xFF;
        assert!(
            !Frame::is_valid(&bad),
            "byte-flip at position {i} must be detected"
        );
    }
}

// ── Bad sync ──────────────────────────────────────────────────────────────────

#[test]
fn bad_sync_is_invalid() {
    let orig = Frame::new(1, FrameType::Data, 0, 1, BROADCAST, [0; 4], 0);
    let mut wire = orig.encode();
    wire[0] = 0x00;
    assert!(!Frame::is_valid(&wire));
    assert!(Frame::decode(&wire).is_err());
}

// ── All frame types roundtrip ─────────────────────────────────────────────────

#[test]
fn all_frame_types_roundtrip() {
    let types = [
        (FrameType::Data,   "DATA"),
        (FrameType::Ack,    "ACK"),
        (FrameType::Beacon, "BEACON"),
        (FrameType::Ctrl,   "CTRL"),
    ];
    for (ft, name) in types {
        let f    = Frame::new(1, ft, 0, 1, 2, [0; 4], 0);
        let wire = f.encode();
        let back = Frame::decode(&wire).unwrap_or_else(|_| panic!("{name} frame must roundtrip"));
        assert_eq!(back.frame_type, ft, "{name} type mismatch");
    }
}

// ── Endianness ────────────────────────────────────────────────────────────────

#[test]
fn seq_big_endian_wire_layout() {
    let f    = Frame::new(1, FrameType::Data, 0x1234, 1, 2, [0; 4], 0);
    let wire = f.encode();
    assert_eq!(wire[2], 0x12, "seq high byte at wire[2]");
    assert_eq!(wire[3], 0x34, "seq low  byte at wire[3]");
}

// ── Timestamp masking ─────────────────────────────────────────────────────────

#[test]
fn timestamp_only_lower_24_bits_stored() {
    let f    = Frame::new(1, FrameType::Data, 0, 1, 2, [0; 4], 0xABCD_EF12);
    let wire = f.encode();
    let back = Frame::decode(&wire).unwrap();
    assert_eq!(back.timestamp_us, 0x00CD_EF12, "only lower 24 bits stored");
}

// ── Cross-language CRC pin (canonical) ───────────────────────────────────────
//
// Verified against:
//   C    dcf_crc16()      (test_conformance.cpp)
//   Haskell crc16ccitt()  (FrameSpec.hs)
//   Lisp  crc16-ccitt     (hydramesh.lisp)

#[test]
fn crc_cross_language_pin_0x42dd() {
    let body: [u8; 15] = [
        0xD3, 0x10, 0x00, 0x01,
        0x00, 0x01, 0xFF, 0xFF,
        0xDE, 0xAD, 0xBE, 0xEF,
        0x00, 0x00, 0x00,
    ];
    assert_eq!(
        crc16_ccitt(&body), 0x42DD,
        "CRC pin failed — algorithm diverged from cross-language reference (0x42DD)"
    );
}
