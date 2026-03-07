// tests/frame_tests.rs — DeMoD 17-byte transport frame unit tests
// DeMoD LLC | GPL-3.0
//
// Rust port of test_frame.cpp.
//
// Run:
//   cargo test --test frame_tests

use demod::frame::{Frame, FrameType, BROADCAST, FRAME_SIZE};

// ── Size ──────────────────────────────────────────────────────────────────────

#[test]
fn frame_wire_is_17_bytes() {
    // The encode() method always produces exactly FRAME_SIZE bytes.
    let f    = Frame::new(1, FrameType::Data, 0, 1, BROADCAST, [0; 4], 0);
    let wire = f.encode();
    assert_eq!(wire.len(), FRAME_SIZE, "encoded frame must be exactly 17 bytes");
}

// ── Init, seal, and validate ──────────────────────────────────────────────────

#[test]
fn init_and_seal() {
    let f = Frame::new(
        1,
        FrameType::Data,
        42,                        // seq
        0x0001,                    // src_id
        0xFFFF,                    // dst_id (broadcast)
        [0xDE, 0xAD, 0xBE, 0xEF], // payload
        0x00AB_1234,               // timestamp_us
    );
    let wire = f.encode();

    // Sync byte
    assert_eq!(wire[0], 0xD3, "sync byte must be 0xD3");

    // Flags: version=1 in high nibble, Data=0 in low nibble → 0x10
    assert_eq!(wire[1], 0x10, "flags must be 0x10 (version=1, type=DATA)");

    // seq = 42 = 0x002A  BE
    assert_eq!(wire[2], 0x00, "seq high byte");
    assert_eq!(wire[3], 0x2A, "seq low byte (42 = 0x2A)");

    // Timestamp 0x00AB1234 → bytes 0xAB, 0x12, 0x34
    assert_eq!(wire[12], 0xAB);
    assert_eq!(wire[13], 0x12);
    assert_eq!(wire[14], 0x34);

    // Roundtrip validation
    let back = Frame::decode(&wire).expect("sealed frame must decode successfully");
    assert_eq!(back.version,      1);
    assert_eq!(back.frame_type,   FrameType::Data);
    assert_eq!(back.seq,          42);
    assert_eq!(back.timestamp_us, 0x00AB_1234);
    assert_eq!(back.payload,      [0xDE, 0xAD, 0xBE, 0xEF]);
}

// ── Corruption detection ──────────────────────────────────────────────────────

#[test]
fn payload_bit_flip_detected() {
    let f = Frame::new(1, FrameType::Beacon, 1000, 0x0002, 0x0001, [0; 4], 0);
    let mut wire = f.encode();
    assert!(Frame::is_valid(&wire), "original frame must be valid");

    wire[10] ^= 0x01; // flip a payload bit
    assert!(
        !Frame::is_valid(&wire),
        "single payload bit flip must be detected by CRC"
    );
}

#[test]
fn bad_sync_byte_detected() {
    let f    = Frame::new(1, FrameType::Beacon, 1000, 0x0002, 0x0001, [0; 4], 0);
    let mut wire = f.encode();
    wire[0] = 0x00;
    assert!(!Frame::is_valid(&wire), "bad sync byte must fail validation");
}

// ── Sequence number endianness ────────────────────────────────────────────────

#[test]
fn seq_big_endian_wire_layout() {
    let f    = Frame::new(1, FrameType::Ack, 0xABCD, 0x0003, 0x0003, [0; 4], 0);
    let wire = f.encode();

    // seq 0xABCD: high byte first
    assert_eq!(wire[2], 0xAB, "SEQ high byte must be at wire[2]");
    assert_eq!(wire[3], 0xCD, "SEQ low byte must be at wire[3]");

    let back = Frame::decode(&wire).unwrap();
    assert_eq!(back.seq, 0xABCD);
}

// ── Timestamp boundary conditions ─────────────────────────────────────────────

#[test]
fn timestamp_zero() {
    let f    = Frame::new(1, FrameType::Data, 0, 1, 2, [0; 4], 0);
    let wire = f.encode();
    let back = Frame::decode(&wire).unwrap();
    assert_eq!(back.timestamp_us, 0);
}

#[test]
fn timestamp_max_24bit() {
    let f    = Frame::new(1, FrameType::Data, 0, 1, 2, [0; 4], 0x00FF_FFFF);
    let wire = f.encode();
    let back = Frame::decode(&wire).unwrap();
    assert_eq!(back.timestamp_us, 0x00FF_FFFF, "max 24-bit timestamp must survive roundtrip");
}

#[test]
fn timestamp_upper_byte_masked() {
    // The frame only stores 24 bits; the upper byte of a 32-bit value is lost.
    let f    = Frame::new(1, FrameType::Data, 0, 1, 2, [0; 4], 0xDEAD_BEEF);
    let wire = f.encode();
    let back = Frame::decode(&wire).unwrap();
    assert_eq!(back.timestamp_us, 0x00AD_BEEF);
}
