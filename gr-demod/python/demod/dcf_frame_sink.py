#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# gr-demod/python/demod/dcf_frame_sink.py
# DeMoD LLC | GPL-3.0
#
# GNU Radio OOT block: DCF Frame Sink
#
# Accepts a stream of bytes (from a packed-bits-to-bytes converter or a
# correlate-access-code block), scans for valid 17-byte DCF frames, and
# emits them as Python message PDUs on the 'frames' message port.
#
# Connection example in GRC:
#
#   [Correlate Access Code - Tag Stream]  →  [Stream to Tagged Stream]
#           │  tagged byte stream
#           ▼
#   [DeMoD DCF Frame Sink]
#           │  message port: 'frames'
#           ▼
#   [Message Debug] / custom handler
#
# Message format (PMT dictionary):
#   pmt.dict with keys:
#     "version"     → pmt.to_long(int)
#     "type"        → pmt.intern(str)   e.g. "DATA", "ACK", "BEACON", "CTRL"
#     "seq"         → pmt.to_long(int)
#     "src_id"      → pmt.to_long(int)
#     "dst_id"      → pmt.to_long(int)
#     "payload"     → pmt.init_u8vector(4, bytes)
#     "timestamp_us"→ pmt.to_long(int)
#     "raw"         → pmt.init_u8vector(17, bytes)   (full wire bytes)
#
# Build requirements:
#   - GNU Radio 3.10+ (Python blocks only, no C++ required)
#   - pip install gnuradio  (or system gnuradio package)
#
# Install:
#   cp dcf_frame_sink.py  <prefix>/lib/python3/dist-packages/demod/
#   cp dcf_frame_source.py <prefix>/lib/python3/dist-packages/demod/

import struct
import numpy as np
from gnuradio import gr
import pmt

# ── CRC-CCITT (poly 0x1021, init 0xFFFF) ─────────────────────────────────────
# Identical to C, Haskell, Rust, Lisp, and Go implementations.

def _crc16_ccitt(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021) if (crc & 0x8000) else (crc << 1)
        crc &= 0xFFFF
    return crc

_FRAME_SIZE  = 17
_SYNC_BYTE   = 0xD3
_TYPE_NAMES  = {0: "DATA", 1: "ACK", 2: "BEACON", 3: "CTRL"}

def _decode_frame(wire: bytes):
    """Return a dict of fields or None if the frame is invalid."""
    if len(wire) != _FRAME_SIZE:
        return None
    if wire[0] != _SYNC_BYTE:
        return None
    stored_crc = (wire[15] << 8) | wire[16]
    if _crc16_ccitt(wire[:15]) != stored_crc:
        return None
    type_nib = wire[1] & 0x0F
    return {
        "version":      (wire[1] >> 4) & 0x0F,
        "type":         _TYPE_NAMES.get(type_nib, f"RSVD({type_nib})"),
        "seq":          (wire[2] << 8) | wire[3],
        "src_id":       (wire[4] << 8) | wire[5],
        "dst_id":       (wire[6] << 8) | wire[7],
        "payload":      bytes(wire[8:12]),
        "timestamp_us": (wire[12] << 16) | (wire[13] << 8) | wire[14],
        "raw":          bytes(wire),
    }


class dcf_frame_sink(gr.sync_block):
    """
    DeMoD DCF Frame Sink.

    Inputs:  1 byte stream  (np.uint8)
    Outputs: 0 stream outputs
    Messages: 'frames' port emits PMT dictionaries for each valid frame.

    The block maintains a sliding window of the last 17 bytes and emits a
    frame message whenever the window contains a valid sync + CRC.

    Parameters:
        verbose (bool): print decoded frames to stdout (default False)
    """

    def __init__(self, verbose=False):
        gr.sync_block.__init__(
            self,
            name="DeMoD DCF Frame Sink",
            in_sig=[np.uint8],
            out_sig=[]
        )
        self.message_port_register_out(pmt.intern("frames"))
        self._verbose   = verbose
        self._buf       = bytearray()
        self._rx_count  = 0

    def work(self, input_items, output_items):
        in0 = input_items[0]
        self._buf.extend(in0)

        # Scan for valid frames with a sliding byte window
        i = 0
        while i <= len(self._buf) - _FRAME_SIZE:
            if self._buf[i] == _SYNC_BYTE:
                candidate = bytes(self._buf[i:i + _FRAME_SIZE])
                fields = _decode_frame(candidate)
                if fields is not None:
                    self._emit(fields)
                    i += _FRAME_SIZE   # advance past the consumed frame
                    continue
            i += 1

        # Keep only the last 16 bytes (incomplete potential frame)
        if len(self._buf) > _FRAME_SIZE - 1:
            self._buf = self._buf[-((_FRAME_SIZE - 1)):]

        return len(in0)

    def _emit(self, fields: dict):
        self._rx_count += 1
        if self._verbose:
            print(
                f"[dcf-sink #{self._rx_count:04d}]"
                f" ver={fields['version']}"
                f" type={fields['type']:<6s}"
                f" seq={fields['seq']:05d}"
                f" src=0x{fields['src_id']:04X}"
                f" dst=0x{fields['dst_id']:04X}"
                f" payload={fields['payload'].hex().upper()}"
                f" ts={fields['timestamp_us']}"
            )

        d = pmt.make_dict()
        d = pmt.dict_add(d, pmt.intern("version"),
                         pmt.from_long(fields["version"]))
        d = pmt.dict_add(d, pmt.intern("type"),
                         pmt.intern(fields["type"]))
        d = pmt.dict_add(d, pmt.intern("seq"),
                         pmt.from_long(fields["seq"]))
        d = pmt.dict_add(d, pmt.intern("src_id"),
                         pmt.from_long(fields["src_id"]))
        d = pmt.dict_add(d, pmt.intern("dst_id"),
                         pmt.from_long(fields["dst_id"]))
        d = pmt.dict_add(d, pmt.intern("payload"),
                         pmt.init_u8vector(4, list(fields["payload"])))
        d = pmt.dict_add(d, pmt.intern("timestamp_us"),
                         pmt.from_long(fields["timestamp_us"]))
        d = pmt.dict_add(d, pmt.intern("raw"),
                         pmt.init_u8vector(_FRAME_SIZE, list(fields["raw"])))

        self.message_port_pub(pmt.intern("frames"), d)
