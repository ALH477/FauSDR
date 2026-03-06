/*
 * transport/demod_frame.h — DeMoD 17-Byte Transport Frame
 *
 * Frame layout (136 bits total, packed):
 * ┌────────┬─────────┬────────────┬────────┬────────┬─────────────┬───────────┬────────┐
 * │  SYNC  │  FLAGS  │    SEQ     │ SRC_ID │ DST_ID │   PAYLOAD   │ TIMESTAMP │ CRC16  │
 * │ 1 byte │ 1 byte  │  2 bytes   │ 2 bytes│ 2 bytes│   4 bytes   │  3 bytes  │ 2 bytes│
 * │  0xD3  │vvvvtttt │ big-endian │        │        │ application │  24-bit µs│CCITT   │
 * └────────┴─────────┴────────────┴────────┴────────┴─────────────┴───────────┴────────┘
 *  v = version (4 bits)
 *  t = frame_type (4 bits): 0=DATA 1=ACK 2=BEACON 3=CTRL 4-15=reserved
 *
 * CRC-CCITT (poly 0x1021, init 0xFFFF) covers bytes [0..14] (excludes CRC field itself).
 * Timestamp is a 24-bit big-endian microsecond offset, wraps ~16.7 seconds.
 *
 * DeMoD LLC
 */

#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// ── Frame constants ───────────────────────────────────────────────────────────
#define DEMOD_FRAME_SYNC       0xD3u
#define DEMOD_FRAME_SIZE       17u
#define DEMOD_FRAME_CRC_LEN    15u     // bytes covered by CRC (0..14)
#define DEMOD_VERSION_SHIFT    4u
#define DEMOD_VERSION_MASK     0xF0u
#define DEMOD_TYPE_MASK        0x0Fu

// Frame type identifiers
typedef enum {
    DEMOD_TYPE_DATA    = 0x0,
    DEMOD_TYPE_ACK     = 0x1,
    DEMOD_TYPE_BEACON  = 0x2,
    DEMOD_TYPE_CTRL    = 0x3,
} demod_frame_type_t;

// ── Frame struct (packed, exactly 17 bytes) ───────────────────────────────────
#pragma pack(push, 1)
typedef struct {
    uint8_t  sync;           // 0xD3 fixed preamble
    uint8_t  flags;          // [7:4]=version, [3:0]=frame_type
    uint16_t seq;            // big-endian rolling 16-bit sequence number
    uint16_t src_id;         // source node identifier
    uint16_t dst_id;         // destination node identifier (0xFFFF = broadcast)
    uint8_t  payload[4];     // application-layer payload bytes
    uint8_t  timestamp[3];   // 24-bit big-endian microsecond offset
    uint16_t crc16;          // CRC-CCITT over bytes [0..14]
} demod_frame_t;
#pragma pack(pop)

#ifdef __cplusplus
static_assert(sizeof(demod_frame_t) == DEMOD_FRAME_SIZE,
              "demod_frame_t must be exactly 17 bytes");
#endif

// ── CRC-CCITT (poly=0x1021, init=0xFFFF) ─────────────────────────────────────
static inline uint16_t demod_crc16(const uint8_t* data, size_t len) {
    uint16_t crc = 0xFFFFu;
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x8000u) ? (uint16_t)((crc << 1) ^ 0x1021u) : (uint16_t)(crc << 1);
    }
    return crc;
}

// ── Frame initialization ──────────────────────────────────────────────────────
static inline void demod_frame_init(demod_frame_t* f,
                                     uint8_t version,
                                     demod_frame_type_t type,
                                     uint16_t src,
                                     uint16_t dst) {
    memset(f, 0, sizeof(*f));
    f->sync   = DEMOD_FRAME_SYNC;
    f->flags  = (uint8_t)(((version & 0xFu) << DEMOD_VERSION_SHIFT) | (type & 0xFu));
    f->src_id = src;
    f->dst_id = dst;
}

// ── Sequence number ───────────────────────────────────────────────────────────
static inline void demod_frame_set_seq(demod_frame_t* f, uint16_t seq) {
    // Store big-endian
    f->seq = (uint16_t)((seq >> 8) | (seq << 8));
}

static inline uint16_t demod_frame_get_seq(const demod_frame_t* f) {
    return (uint16_t)((f->seq >> 8) | (f->seq << 8));
}

// ── Timestamp (24-bit µs, big-endian) ────────────────────────────────────────
static inline void demod_frame_set_ts(demod_frame_t* f, uint32_t us24) {
    f->timestamp[0] = (uint8_t)((us24 >> 16) & 0xFFu);
    f->timestamp[1] = (uint8_t)((us24 >>  8) & 0xFFu);
    f->timestamp[2] = (uint8_t)( us24         & 0xFFu);
}

static inline uint32_t demod_frame_get_ts(const demod_frame_t* f) {
    return ((uint32_t)f->timestamp[0] << 16)
         | ((uint32_t)f->timestamp[1] <<  8)
         |  (uint32_t)f->timestamp[2];
}

// ── Seal (compute & write CRC) ────────────────────────────────────────────────
static inline void demod_frame_seal(demod_frame_t* f) {
    uint16_t crc = demod_crc16((const uint8_t*)f, DEMOD_FRAME_CRC_LEN);
    // Store big-endian
    f->crc16 = (uint16_t)((crc >> 8) | (crc << 8));
}

// ── Validate (check sync + CRC) ───────────────────────────────────────────────
static inline bool demod_frame_validate(const demod_frame_t* f) {
    if (f->sync != DEMOD_FRAME_SYNC) return false;
    uint16_t crc = demod_crc16((const uint8_t*)f, DEMOD_FRAME_CRC_LEN);
    uint16_t stored = (uint16_t)((f->crc16 >> 8) | (f->crc16 << 8));
    return crc == stored;
}

// ── Accessor helpers ──────────────────────────────────────────────────────────
static inline uint8_t  demod_frame_version(const demod_frame_t* f) {
    return (f->flags & DEMOD_VERSION_MASK) >> DEMOD_VERSION_SHIFT;
}
static inline demod_frame_type_t demod_frame_type(const demod_frame_t* f) {
    return (demod_frame_type_t)(f->flags & DEMOD_TYPE_MASK);
}
