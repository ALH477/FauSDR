#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# gr-demod/python/demod/qa_dcf_frame.py
# DeMoD LLC | GPL-3.0
#
# Unit tests for dcf_frame_sink and dcf_frame_source.
# Run: python3 -m pytest qa_dcf_frame.py -v
#
# Requires: gnuradio (or mock), pytest

import struct
import unittest

# ── Import the codec logic directly (no GNU Radio required for unit tests) ─────

def _crc16_ccitt(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021) if (crc & 0x8000) else (crc << 1)
        crc &= 0xFFFF
    return crc

FRAME_SIZE = 17
SYNC_BYTE  = 0xD3
TYPE_NAMES = {0: "DATA", 1: "ACK", 2: "BEACON", 3: "CTRL"}

def encode_frame(version, ftype, seq, src_id, dst_id, payload, ts_us):
    w = bytearray(FRAME_SIZE)
    w[0] = SYNC_BYTE
    w[1] = ((version & 0x0F) << 4) | (ftype & 0x0F)
    w[2] = (seq >> 8) & 0xFF
    w[3] = seq & 0xFF
    w[4] = (src_id >> 8) & 0xFF
    w[5] = src_id & 0xFF
    w[6] = (dst_id >> 8) & 0xFF
    w[7] = dst_id & 0xFF
    w[8:12] = payload[:4]
    w[12] = (ts_us >> 16) & 0xFF
    w[13] = (ts_us >>  8) & 0xFF
    w[14] = ts_us & 0xFF
    crc = _crc16_ccitt(bytes(w[:15]))
    w[15] = (crc >> 8) & 0xFF
    w[16] = crc & 0xFF
    return bytes(w)

def decode_frame(wire):
    if len(wire) != FRAME_SIZE or wire[0] != SYNC_BYTE:
        return None
    stored = (wire[15] << 8) | wire[16]
    if _crc16_ccitt(wire[:15]) != stored:
        return None
    return {
        "version":      (wire[1] >> 4) & 0x0F,
        "type":         TYPE_NAMES.get(wire[1] & 0x0F, "RSVD"),
        "seq":          (wire[2] << 8) | wire[3],
        "src_id":       (wire[4] << 8) | wire[5],
        "dst_id":       (wire[6] << 8) | wire[7],
        "payload":      bytes(wire[8:12]),
        "timestamp_us": (wire[12] << 16) | (wire[13] << 8) | wire[14],
    }


class TestCRCPin(unittest.TestCase):
    """CRC pin 0x42DD — cross-language conformance."""

    def test_reference_vector(self):
        """
        Canonical reference vector: version=1, DATA, seq=1, src=1,
        dst=0xFFFF, payload=DEADBEEF, ts=0 → last two bytes must be 0x42 0xDD.
        """
        wire = encode_frame(1, 0, 1, 1, 0xFFFF, b'\xDE\xAD\xBE\xEF', 0)
        self.assertEqual(len(wire), 17)
        self.assertEqual(wire[0],  0xD3, "sync byte")
        self.assertEqual(wire[1],  0x10, "flags ver=1 type=DATA")
        self.assertEqual(wire[2],  0x00, "seq hi")
        self.assertEqual(wire[3],  0x01, "seq lo")
        self.assertEqual(wire[4],  0x00, "src_id hi")
        self.assertEqual(wire[5],  0x01, "src_id lo")
        self.assertEqual(wire[6],  0xFF, "dst_id hi")
        self.assertEqual(wire[7],  0xFF, "dst_id lo")
        self.assertEqual(wire[8],  0xDE, "payload[0]")
        self.assertEqual(wire[9],  0xAD, "payload[1]")
        self.assertEqual(wire[10], 0xBE, "payload[2]")
        self.assertEqual(wire[11], 0xEF, "payload[3]")
        self.assertEqual(wire[15], 0x42, "CRC hi pin")
        self.assertEqual(wire[16], 0xDD, "CRC lo pin")


class TestRoundTrip(unittest.TestCase):

    def test_encode_decode(self):
        wire = encode_frame(1, 0, 42, 3, 7, b'\x11\x22\x33\x44', 0xABCDEF)
        f = decode_frame(wire)
        self.assertIsNotNone(f)
        self.assertEqual(f["version"], 1)
        self.assertEqual(f["type"], "DATA")
        self.assertEqual(f["seq"], 42)
        self.assertEqual(f["src_id"], 3)
        self.assertEqual(f["dst_id"], 7)
        self.assertEqual(f["payload"], b'\x11\x22\x33\x44')
        self.assertEqual(f["timestamp_us"], 0xABCDEF)

    def test_all_types(self):
        for t, name in TYPE_NAMES.items():
            wire = encode_frame(1, t, 0, 1, 2, b'\x00'*4, 0)
            f = decode_frame(wire)
            self.assertIsNotNone(f)
            self.assertEqual(f["type"], name)

    def test_broadcast(self):
        wire = encode_frame(1, 0, 0, 1, 0xFFFF, b'\x00'*4, 0)
        f = decode_frame(wire)
        self.assertEqual(f["dst_id"], 0xFFFF)


class TestValidation(unittest.TestCase):

    def test_bad_sync(self):
        wire = bytearray(encode_frame(1, 0, 0, 1, 2, b'\x00'*4, 0))
        wire[0] = 0x00
        self.assertIsNone(decode_frame(bytes(wire)))

    def test_crc_corruption(self):
        wire = bytearray(encode_frame(1, 0, 0, 1, 2, b'\x00'*4, 0))
        wire[8] ^= 0xFF   # corrupt payload
        self.assertIsNone(decode_frame(bytes(wire)))

    def test_wrong_length(self):
        self.assertIsNone(decode_frame(b'\x00' * 16))
        self.assertIsNone(decode_frame(b'\x00' * 18))
        self.assertIsNone(decode_frame(b''))

    def test_all_body_bytes_detected(self):
        wire = encode_frame(1, 0, 1, 1, 0xFFFF, b'\xCA\xFE\xBA\xBE', 0)
        for i in range(1, 15):
            bad = bytearray(wire)
            bad[i] ^= 0xFF
            self.assertIsNone(decode_frame(bytes(bad)),
                              f"corruption at byte {i} not detected")


class TestSlidingWindowSync(unittest.TestCase):
    """Simulate the dcf_frame_sink sliding-window scan."""

    def _scan(self, buf):
        results = []
        i = 0
        while i <= len(buf) - FRAME_SIZE:
            if buf[i] == SYNC_BYTE:
                candidate = buf[i:i + FRAME_SIZE]
                f = decode_frame(bytes(candidate))
                if f:
                    results.append(f)
                    i += FRAME_SIZE
                    continue
            i += 1
        return results

    def test_single_frame_in_stream(self):
        wire = encode_frame(1, 0, 7, 1, 0xFFFF, b'\x01\x02\x03\x04', 0)
        buf = bytes(10) + wire + bytes(10)
        frames = self._scan(bytearray(buf))
        self.assertEqual(len(frames), 1)
        self.assertEqual(frames[0]["seq"], 7)

    def test_two_frames_back_to_back(self):
        w1 = encode_frame(1, 0, 1, 1, 0xFFFF, b'\x01\x00\x00\x00', 0)
        w2 = encode_frame(1, 0, 2, 1, 0xFFFF, b'\x02\x00\x00\x00', 0)
        buf = bytearray(w1 + w2)
        frames = self._scan(buf)
        self.assertEqual(len(frames), 2)
        self.assertEqual(frames[0]["seq"], 1)
        self.assertEqual(frames[1]["seq"], 2)

    def test_frame_with_leading_junk(self):
        junk = bytes([0xD3, 0x00, 0x00])   # false sync, too short for frame
        wire = encode_frame(1, 2, 99, 1, 0xFFFF, b'\xFF\xFF\xFF\xFF', 0)
        buf = bytearray(junk + wire)
        frames = self._scan(buf)
        self.assertEqual(len(frames), 1)
        self.assertEqual(frames[0]["seq"], 99)


if __name__ == "__main__":
    unittest.main()
