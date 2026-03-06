// src/test_frame.cpp — DeMoD 17-byte transport frame unit tests
// cmake --build build && ctest --test-dir build -V

#include "demod_frame.h"

#include <cassert>
#include <cstdio>
#include <cstring>

static int tests_run = 0, tests_passed = 0;

#define CHECK(expr) do { \
    tests_run++; \
    if (!(expr)) { \
        fprintf(stderr, "FAIL [line %d]: %s\n", __LINE__, #expr); \
    } else { \
        tests_passed++; \
    } \
} while(0)

static void test_size() {
    CHECK(sizeof(demod_frame_t) == 17);
}

static void test_init_and_seal() {
    demod_frame_t f;
    demod_frame_init(&f, 1, DEMOD_TYPE_DATA, 0x0001, 0xFFFF);
    demod_frame_set_seq(&f, 42);
    demod_frame_set_ts(&f, 0x00AB1234);

    f.payload[0] = 0xDE;
    f.payload[1] = 0xAD;
    f.payload[2] = 0xBE;
    f.payload[3] = 0xEF;

    demod_frame_seal(&f);

    CHECK(demod_frame_validate(&f));
    CHECK(f.sync == 0xD3);
    CHECK(demod_frame_version(&f) == 1);
    CHECK(demod_frame_type(&f) == DEMOD_TYPE_DATA);
    CHECK(demod_frame_get_seq(&f) == 42);
    CHECK(demod_frame_get_ts(&f) == 0x00AB1234);
}

static void test_corruption_detection() {
    demod_frame_t f;
    demod_frame_init(&f, 1, DEMOD_TYPE_BEACON, 0x0002, 0x0001);
    demod_frame_set_seq(&f, 1000);
    demod_frame_seal(&f);

    CHECK(demod_frame_validate(&f));

    // Flip a payload bit
    demod_frame_t g = f;
    g.payload[2] ^= 0x01;
    CHECK(!demod_frame_validate(&g));

    // Bad sync byte
    demod_frame_t h = f;
    h.sync = 0x00;
    CHECK(!demod_frame_validate(&h));
}

static void test_seq_endian() {
    demod_frame_t f;
    demod_frame_init(&f, 1, DEMOD_TYPE_ACK, 0x0003, 0x0003);
    demod_frame_set_seq(&f, 0xABCD);
    CHECK(demod_frame_get_seq(&f) == 0xABCD);
    // Verify big-endian wire layout
    const uint8_t* raw = (const uint8_t*)&f;
    CHECK(raw[2] == 0xAB);  // SEQ high byte first
    CHECK(raw[3] == 0xCD);
}

static void test_timestamp_boundaries() {
    demod_frame_t f;
    demod_frame_init(&f, 1, DEMOD_TYPE_DATA, 0x0001, 0x0002);

    demod_frame_set_ts(&f, 0);
    CHECK(demod_frame_get_ts(&f) == 0);

    demod_frame_set_ts(&f, 0xFFFFFF);  // max 24-bit
    CHECK(demod_frame_get_ts(&f) == 0xFFFFFF);
}

int main() {
    test_size();
    test_init_and_seal();
    test_corruption_detection();
    test_seq_endian();
    test_timestamp_boundaries();

    fprintf(stdout, "\nFrame tests: %d/%d passed\n", tests_passed, tests_run);
    return (tests_passed == tests_run) ? 0 : 1;
}
