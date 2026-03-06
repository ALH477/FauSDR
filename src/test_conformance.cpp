/*
 * test_conformance.cpp — DeMoD cross-language codec conformance harness (C)
 * DeMoD LLC | GPL-3.0
 *
 * Asserts the canonical reference test vector against the C codec in
 * transport/dcf_frame.h, including the CRC pin 0x42DD.
 *
 * This is the source-of-truth test.  The same vector is pinned in:
 *   Haskell  — haskell/test/FrameSpec.hs  (crcCrossLanguagePin)
 *   Rust     — dcf/rust/src/frame.rs      (#[test] crc_cross_language_pin)
 *   Lisp     — hydramesh.lisp             (dcf-frame-crc-test)
 *
 * If ANY language's test fails the pin, that codec has diverged from the
 * others — a wire-breaking change that must be fixed before deployment.
 *
 * Run:
 *   cmake --build build && ctest --test-dir build -V -R conformance
 */

#include "dcf_frame.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// ── Helpers ───────────────────────────────────────────────────────────────────

static int tests_run    = 0;
static int tests_failed = 0;

#define ASSERT(cond, msg) do {                                      \
    ++tests_run;                                                    \
    if (!(cond)) {                                                  \
        fprintf(stderr, "FAIL [%s:%d] %s\n", __FILE__, __LINE__,   \
                (msg));                                             \
        ++tests_failed;                                             \
    } else {                                                        \
        fprintf(stderr, "ok   %s\n", (msg));                        \
    }                                                               \
} while (0)

#define ASSERT_EQ(a, b, msg) do {                                   \
    ++tests_run;                                                    \
    if ((a) != (b)) {                                               \
        fprintf(stderr, "FAIL [%s:%d] %s  (got 0x%X, want 0x%X)\n",\
                __FILE__, __LINE__, (msg),                          \
                (unsigned)(a), (unsigned)(b));                      \
        ++tests_failed;                                             \
    } else {                                                        \
        fprintf(stderr, "ok   %s\n", (msg));                        \
    }                                                               \
} while (0)

// ── Reference test vector ─────────────────────────────────────────────────────
//
// Canonical input:
//   version=1, type=DATA(0), seq=1, src_id=1, dst_id=0xFFFF,
//   payload={0xDE,0xAD,0xBE,0xEF}, timestamp_us=0
//
// Expected wire bytes (17):
//   D3 10 00 01  00 01 FF FF  DE AD BE EF  00 00 00  42 DD
//
// CRC-CCITT over bytes [0..14] must equal 0x42DD.
// This value is pinned across all language codecs.

static const uint8_t REFERENCE_FRAME[DCF_FRAME_SIZE] = {
    0xD3, 0x10, 0x00, 0x01,   // sync, flags(ver=1,type=DATA), seq BE
    0x00, 0x01, 0xFF, 0xFF,   // src_id, dst_id (broadcast)
    0xDE, 0xAD, 0xBE, 0xEF,  // payload
    0x00, 0x00, 0x00,          // timestamp_us = 0 (24-bit BE)
    0x42, 0xDD                 // CRC16-CCITT pin
};

// ── Tests ─────────────────────────────────────────────────────────────────────

static void test_crc_pin(void) {
    // Compute CRC over body bytes [0..14] and check it matches the pin.
    // Use the internal helper exposed through dcf_frame_valid().
    // We validate the full reference frame — valid() iff CRC matches.
    ASSERT(dcf_frame_valid(REFERENCE_FRAME),
           "CRC pin 0x42DD: dcf_frame_valid(reference_frame) == true");

    // Flip one bit in the CRC itself — must still decode body correctly
    // but validate() must fail.
    uint8_t bad_crc[DCF_FRAME_SIZE];
    memcpy(bad_crc, REFERENCE_FRAME, DCF_FRAME_SIZE);
    bad_crc[15] ^= 0x01;
    ASSERT(!dcf_frame_valid(bad_crc),
           "CRC pin: flipped CRC byte → dcf_frame_valid == false");
}

static void test_reference_vector_encode(void) {
    dcf_frame_t f;
    dcf_frame_init(&f,
        /*version*/  1,
        /*type*/     DCF_TYPE_DATA,
        /*seq*/      1,
        /*src_id*/   0x0001,
        /*dst_id*/   DCF_BROADCAST);
    f.payload[0] = 0xDE; f.payload[1] = 0xAD;
    f.payload[2] = 0xBE; f.payload[3] = 0xEF;
    f.timestamp_us = 0;

    uint8_t wire[DCF_FRAME_SIZE];
    dcf_frame_encode(&f, wire);

    ASSERT(memcmp(wire, REFERENCE_FRAME, DCF_FRAME_SIZE) == 0,
           "Encode: output matches reference vector byte-for-byte");

    // Individual byte checks for diagnostic clarity
    ASSERT_EQ(wire[0],  0xD3, "encode: sync byte == 0xD3");
    ASSERT_EQ(wire[1],  0x10, "encode: flags == 0x10 (ver=1, type=DATA)");
    ASSERT_EQ(wire[2],  0x00, "encode: seq high byte == 0x00");
    ASSERT_EQ(wire[3],  0x01, "encode: seq low byte == 0x01");
    ASSERT_EQ(wire[4],  0x00, "encode: src_id high == 0x00");
    ASSERT_EQ(wire[5],  0x01, "encode: src_id low == 0x01");
    ASSERT_EQ(wire[6],  0xFF, "encode: dst_id high == 0xFF");
    ASSERT_EQ(wire[7],  0xFF, "encode: dst_id low == 0xFF (broadcast)");
    ASSERT_EQ(wire[8],  0xDE, "encode: payload[0] == 0xDE");
    ASSERT_EQ(wire[9],  0xAD, "encode: payload[1] == 0xAD");
    ASSERT_EQ(wire[10], 0xBE, "encode: payload[2] == 0xBE");
    ASSERT_EQ(wire[11], 0xEF, "encode: payload[3] == 0xEF");
    ASSERT_EQ(wire[12], 0x00, "encode: timestamp byte 0 == 0x00");
    ASSERT_EQ(wire[13], 0x00, "encode: timestamp byte 1 == 0x00");
    ASSERT_EQ(wire[14], 0x00, "encode: timestamp byte 2 == 0x00");
    ASSERT_EQ(wire[15], 0x42, "encode: CRC high byte == 0x42 (pin)");
    ASSERT_EQ(wire[16], 0xDD, "encode: CRC low byte == 0xDD (pin)");
}

static void test_reference_vector_decode(void) {
    dcf_frame_t f;
    bool ok = dcf_frame_decode(REFERENCE_FRAME, &f);
    ASSERT(ok, "Decode: dcf_frame_decode(reference_frame) == true");
    ASSERT_EQ(f.version,      1,        "decode: version == 1");
    ASSERT_EQ(f.type,         DCF_TYPE_DATA, "decode: type == DATA");
    ASSERT_EQ(f.seq,          1,        "decode: seq == 1");
    ASSERT_EQ(f.src_id,       0x0001,   "decode: src_id == 1");
    ASSERT_EQ(f.dst_id,       0xFFFF,   "decode: dst_id == 0xFFFF (broadcast)");
    ASSERT_EQ(f.payload[0],   0xDE,     "decode: payload[0] == 0xDE");
    ASSERT_EQ(f.payload[1],   0xAD,     "decode: payload[1] == 0xAD");
    ASSERT_EQ(f.payload[2],   0xBE,     "decode: payload[2] == 0xBE");
    ASSERT_EQ(f.payload[3],   0xEF,     "decode: payload[3] == 0xEF");
    ASSERT_EQ(f.timestamp_us, 0u,       "decode: timestamp_us == 0");
}

static void test_roundtrip(void) {
    dcf_frame_t orig;
    dcf_frame_init(&orig, 1, DCF_TYPE_ACK, 0xABCD, 0x0042, 0x0007);
    orig.payload[0] = 0x11; orig.payload[1] = 0x22;
    orig.payload[2] = 0x33; orig.payload[3] = 0x44;
    orig.timestamp_us = 0xA1B2C3;

    uint8_t wire[DCF_FRAME_SIZE];
    dcf_frame_encode(&orig, wire);

    dcf_frame_t back;
    bool ok = dcf_frame_decode(wire, &back);
    ASSERT(ok,                                          "roundtrip: decode ok");
    ASSERT_EQ(back.version,       orig.version,         "roundtrip: version");
    ASSERT_EQ(back.type,          orig.type,             "roundtrip: type");
    ASSERT_EQ(back.seq,           orig.seq,              "roundtrip: seq");
    ASSERT_EQ(back.src_id,        orig.src_id,           "roundtrip: src_id");
    ASSERT_EQ(back.dst_id,        orig.dst_id,           "roundtrip: dst_id");
    ASSERT_EQ(back.payload[0],    orig.payload[0],       "roundtrip: payload[0]");
    ASSERT_EQ(back.payload[3],    orig.payload[3],       "roundtrip: payload[3]");
    ASSERT_EQ(back.timestamp_us,  orig.timestamp_us & 0xFFFFFF, "roundtrip: timestamp_us (24-bit)");
}

static void test_corruption_detection(void) {
    uint8_t wire[DCF_FRAME_SIZE];
    dcf_frame_t f;
    dcf_frame_init(&f, 1, DCF_TYPE_DATA, 0, 1, 0xFFFF);
    f.payload[0] = 0xCA; f.payload[1] = 0xFE;
    f.payload[2] = 0xBA; f.payload[3] = 0xBE;
    dcf_frame_encode(&f, wire);

    // Flip every body byte (positions 1–14) individually — each must fail.
    int all_detected = 1;
    for (int i = 1; i < 15; i++) {
        uint8_t bad[DCF_FRAME_SIZE];
        memcpy(bad, wire, DCF_FRAME_SIZE);
        bad[i] ^= 0xFF;  // flip all 8 bits
        if (dcf_frame_valid(bad)) {
            fprintf(stderr, "FAIL: corruption at byte %d not detected\n", i);
            all_detected = 0;
        }
    }
    ASSERT(all_detected, "corruption: every single-byte body flip detected");
}

static void test_bad_sync(void) {
    dcf_frame_t f;
    uint8_t wire[DCF_FRAME_SIZE];
    dcf_frame_init(&f, 1, DCF_TYPE_DATA, 0, 1, 0xFFFF);
    dcf_frame_encode(&f, wire);
    wire[0] = 0x00;  // clobber sync byte

    ASSERT(!dcf_frame_valid(wire),      "bad sync: valid() == false");
    ASSERT(!dcf_frame_decode(wire, &f), "bad sync: decode() == false");
}

static void test_all_frame_types(void) {
    const dcf_frame_type_t types[] = {
        DCF_TYPE_DATA, DCF_TYPE_ACK, DCF_TYPE_BEACON, DCF_TYPE_CTRL
    };
    const char* names[] = { "DATA", "ACK", "BEACON", "CTRL" };
    for (int i = 0; i < 4; i++) {
        dcf_frame_t f, back;
        uint8_t wire[DCF_FRAME_SIZE];
        dcf_frame_init(&f, 1, types[i], (uint16_t)i, 1, 2);
        dcf_frame_encode(&f, wire);
        bool ok = dcf_frame_decode(wire, &back);
        char msg[64];
        snprintf(msg, sizeof(msg), "frame type %s roundtrip", names[i]);
        ASSERT(ok && back.type == types[i], msg);
    }
}

static void test_seq_endianness(void) {
    // seq=0x1234 must appear as bytes [0x12, 0x34] at wire positions [2,3]
    dcf_frame_t f;
    uint8_t wire[DCF_FRAME_SIZE];
    dcf_frame_init(&f, 1, DCF_TYPE_DATA, 0x1234, 1, 2);
    dcf_frame_encode(&f, wire);
    ASSERT_EQ(wire[2], 0x12, "seq big-endian: wire[2] == 0x12");
    ASSERT_EQ(wire[3], 0x34, "seq big-endian: wire[3] == 0x34");
}

static void test_timestamp_24bit(void) {
    // Only the lower 24 bits of timestamp_us are stored
    dcf_frame_t f, back;
    uint8_t wire[DCF_FRAME_SIZE];
    dcf_frame_init(&f, 1, DCF_TYPE_DATA, 0, 1, 2);
    f.timestamp_us = 0xABCDEF12;  // upper byte must be masked away
    dcf_frame_encode(&f, wire);
    dcf_frame_decode(wire, &back);
    ASSERT_EQ(back.timestamp_us, 0xCDEF12u, "timestamp: only lower 24 bits stored");
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main(void) {
    fprintf(stderr, "\n=== DeMoD Conformance Test Harness ===\n\n");

    test_crc_pin();
    test_reference_vector_encode();
    test_reference_vector_decode();
    test_roundtrip();
    test_corruption_detection();
    test_bad_sync();
    test_all_frame_types();
    test_seq_endianness();
    test_timestamp_24bit();

    fprintf(stderr, "\n%d tests run, %d failed.\n\n", tests_run, tests_failed);

    if (tests_failed > 0) {
        fprintf(stderr,
            "CONFORMANCE FAILURE: C codec has diverged from reference.\n"
            "Check crc16_ccitt() implementation and field byte order.\n"
            "All language codecs must produce identical bytes for the same input.\n\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr,
        "All conformance tests passed.\n"
        "CRC pin 0x42DD verified. Reference vector matches byte-for-byte.\n\n");
    return EXIT_SUCCESS;
}
