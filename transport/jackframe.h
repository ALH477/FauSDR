/*
 * jackframe.h — DeMoD JackFrame: 8-byte low-latency mini-frame
 * DeMoD LLC | GPL-3.0
 *
 * Designed for audio-band BPSK modems (guitar cable / USB audio interface).
 * Minimises frame airtime at the cost of routing metadata.
 *
 * Wire layout (8 bytes = 64 bits):
 *
 *   Byte  Field    Width  Notes
 *   ────  ───────  ─────  ────────────────────────────────────────────────────
 *    0    sync     1 B    Fixed 0xA5 — first validity gate
 *    1    header   1 B    [7:4] seq_nibble (0–15, rolling) | [3:0] type (0–7)
 *    2-5  payload  4 B    Application data: float32 | uint32 | 4×uint8
 *    6-7  crc16    2 B    CRC-CCITT(0x1021, init=0xFFFF) over bytes [0..5]
 *
 * Frame types:
 *   0 = DATA    — raw payload
 *   1 = ACK     — acknowledgement (payload = echoed seq + status)
 *   2 = BEACON  — keepalive / clock sync
 *   3 = CTRL    — control message
 *
 * Latency budget @ 9600 baud, 96 kHz SR, 32-sample buffer:
 *   Frame airtime:  64 / 9600  = 6.667 ms
 *   TX buffer fill: 32 / 96000 = 0.333 ms
 *   One-way total:              ≈ 7.0 ms
 *   Jitter:        ±1 sample   = ±0.010 ms  (vs ±50 ms Ethernet under load)
 *
 * Reference vector (DATA, seq=0, payload=0xDEADBEEF):
 *   Wire: 0xA5 0x01 0xDE 0xAD 0xBE 0xEF 0xC2 0x3F
 *   CRC pin: 0xC23F
 *
 * Relationship to DCF frame (dcf_frame.h):
 *   JackFrame is a point-to-point micro-frame for latency-critical links.
 *   It carries no routing (src/dst/timestamp). Bridge to DCF by wrapping
 *   the 4-byte payload inside a DCF DATA frame on the receiving end.
 *
 * Compatible with:
 *   - USB audio interfaces (Focusrite Scarlett, Behringer UMC series)
 *   - Standard TS/TRS guitar cables (usable BW ~15–18 kHz at line level)
 *   - JACK2 / PipeWire real-time audio backends
 *   - ArchibaldOS (CachyOS RT kernel, sub-millisecond scheduling)
 */

#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ── Constants ───────────────────────────────────────────────────────────── */

#define JF_FRAME_SIZE      8u     /* total wire bytes                        */
#define JF_CRC_COVER       6u     /* bytes covered by CRC: [0..5]            */
#define JF_SYNC_BYTE       0xA5u  /* fixed sync — different from DCF 0xD3   */
#define JF_SEQ_SHIFT       4u
#define JF_SEQ_MASK        0xF0u
#define JF_TYPE_MASK       0x0Fu

typedef enum {
    JF_TYPE_DATA   = 0,
    JF_TYPE_ACK    = 1,
    JF_TYPE_BEACON = 2,
    JF_TYPE_CTRL   = 3,
} jf_type_t;

typedef struct {
    uint8_t  seq;          /* 4-bit rolling counter (0–15)                   */
    jf_type_t type;
    uint8_t  payload[4];   /* raw application bytes                          */
} jack_frame_t;

/* ── CRC-CCITT (poly=0x1021, init=0xFFFF) — same as DCF ──────────────────── */
static inline uint16_t jf_crc16(const uint8_t *d, size_t n) {
    uint16_t crc = 0xFFFFu;
    for (size_t i = 0; i < n; i++) {
        crc ^= (uint16_t)((uint16_t)d[i] << 8);
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x8000u)
                ? (uint16_t)((crc << 1) ^ 0x1021u)
                : (uint16_t) (crc << 1);
    }
    return crc;
}

/* ── Encode ──────────────────────────────────────────────────────────────── */
static inline uint8_t *jf_encode(const jack_frame_t *f, uint8_t *buf) {
    buf[0] = JF_SYNC_BYTE;
    buf[1] = (uint8_t)(((f->seq & 0x0Fu) << JF_SEQ_SHIFT)
                      | (f->type & 0x0Fu));
    memcpy(buf + 2, f->payload, 4);
    uint16_t crc = jf_crc16(buf, JF_CRC_COVER);
    buf[6] = (uint8_t)(crc >> 8);
    buf[7] = (uint8_t)(crc & 0xFF);
    return buf;
}

/* ── Decode ──────────────────────────────────────────────────────────────── */
static inline bool jf_decode(const uint8_t *buf, jack_frame_t *out) {
    if (buf[0] != JF_SYNC_BYTE) return false;
    uint16_t crc_calc   = jf_crc16(buf, JF_CRC_COVER);
    uint16_t crc_stored = (uint16_t)(((uint16_t)buf[6] << 8) | buf[7]);
    if (crc_calc != crc_stored) return false;
    out->seq  = (buf[1] & JF_SEQ_MASK) >> JF_SEQ_SHIFT;
    out->type = (jf_type_t)(buf[1] & JF_TYPE_MASK);
    memcpy(out->payload, buf + 2, 4);
    return true;
}

/* ── Validate ────────────────────────────────────────────────────────────── */
static inline bool jf_valid(const uint8_t *buf) {
    if (buf[0] != JF_SYNC_BYTE) return false;
    uint16_t c = jf_crc16(buf, JF_CRC_COVER);
    return (uint8_t)(c >> 8) == buf[6] && (uint8_t)(c & 0xFF) == buf[7];
}

/* ── Float32 convenience helpers ─────────────────────────────────────────── */
static inline void jf_set_float(jack_frame_t *f, float v) {
    memcpy(f->payload, &v, 4);
}
static inline float jf_get_float(const jack_frame_t *f) {
    float v; memcpy(&v, f->payload, 4); return v;
}

/* ── DCF bridge: wrap JackFrame payload into a DCF DATA frame ────────────── */
/*
 * Use this on the RX side to forward JackFrame payloads into the DCF network.
 * #include "dcf_frame.h" before using.
 *
 * static inline void jf_to_dcf(const jack_frame_t *jf, dcf_frame_t *dcf,
 *                               uint16_t src, uint16_t dst, uint16_t seq) {
 *     dcf_frame_init(dcf, 1, DCF_TYPE_DATA, seq, src, dst);
 *     memcpy(dcf->payload, jf->payload, 4);
 * }
 */
