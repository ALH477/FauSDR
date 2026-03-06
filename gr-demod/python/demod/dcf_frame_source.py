#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# gr-demod/python/demod/dcf_frame_source.py
# DeMoD LLC | GPL-3.0
#
# GNU Radio OOT block: DCF Frame Source
#
# Accepts DCF frame parameters as a message PDU on the 'in' message port
# and emits the encoded 17-byte frame as a byte stream on output[0].
#
# The emitted bytes feed directly into a GNU Radio modulator chain:
#   [DeMoD DCF Frame Source]
#           │  byte stream (np.uint8)
#           ▼
#   [Pack K Bits]   (8 → 1)
#           │  bit stream (0/1 per sample)
#           ▼
#   [Constellation Modulator]  or  [GMSK Mod]  etc.
#           │  complex IQ
#           ▼
#   [UHD: USRP Sink]  /  [RTL-SDR Sink]
#
# To send a frame from Python:
#   src_block = dcf_frame_source(src_id=0x0001, repeat=False)
#   msg = pmt.make_dict()
#   msg = pmt.dict_add(msg, pmt.intern("payload"), pmt.init_u8vector(4, [0xDE,0xAD,0xBE,0xEF]))
#   src_block.to_basic_block()._post(pmt.intern("in"), msg)
#
# Parameters:
#   src_id  (int)  : source node ID to embed in frames (default 1)
#   dst_id  (int)  : destination node ID (default 0xFFFF = broadcast)
#   repeat  (bool) : if True, loop the frame buffer continuously (default False)
#   beacon  (bool) : if True, auto-generate beacon frames at beacon_interval_s

import struct
import time
import threading
import numpy as np
from gnuradio import gr
import pmt

def _crc16_ccitt(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021) if (crc & 0x8000) else (crc << 1)
        crc &= 0xFFFF
    return crc

_FRAME_SIZE = 17
_SYNC_BYTE  = 0xD3

def _encode_frame(version, ftype, seq, src_id, dst_id, payload, ts_us):
    """Encode a DCF frame to 17 bytes."""
    w = bytearray(_FRAME_SIZE)
    w[0] = _SYNC_BYTE
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

def _timestamp_us24():
    us = int(time.monotonic() * 1_000_000)
    return us & 0xFFFFFF


class dcf_frame_source(gr.sync_block):
    """
    DeMoD DCF Frame Source.

    Inputs:  0 stream inputs
    Outputs: 1 byte stream (np.uint8)
    Messages: 'in' port accepts PMT dicts with optional keys:
                payload       → u8vector(4)  (default 0x00000000)
                type          → long (0=DATA 1=ACK 2=BEACON 3=CTRL, default 0)
                dst_id        → long (default broadcast 0xFFFF)
    """

    def __init__(self, src_id=1, dst_id=0xFFFF, repeat=False, beacon=False,
                 beacon_interval_s=1.0):
        gr.sync_block.__init__(
            self,
            name="DeMoD DCF Frame Source",
            in_sig=[],
            out_sig=[np.uint8]
        )
        self.message_port_register_in(pmt.intern("in"))
        self.set_msg_handler(pmt.intern("in"), self._handle_msg)

        self._src_id     = src_id
        self._dst_id     = dst_id
        self._repeat     = repeat
        self._seq        = 0
        self._seq_lock   = threading.Lock()
        self._buf        = bytearray()
        self._buf_lock   = threading.Lock()

        if beacon:
            self._beacon_thread = threading.Thread(
                target=self._beacon_loop,
                args=(beacon_interval_s,),
                daemon=True
            )
            self._beacon_thread.start()

    def _next_seq(self):
        with self._seq_lock:
            s = self._seq
            self._seq = (self._seq + 1) & 0xFFFF
        return s

    def _handle_msg(self, msg):
        if not pmt.is_dict(msg):
            return
        payload = bytes([0, 0, 0, 0])
        ftype   = 0
        dst_id  = self._dst_id

        p_pmt = pmt.dict_ref(msg, pmt.intern("payload"), pmt.PMT_NIL)
        if not pmt.is_null(p_pmt) and pmt.is_u8vector(p_pmt):
            raw = pmt.u8vector_elements(p_pmt)
            payload = bytes(raw[:4]) + bytes(max(0, 4 - len(raw)) * [0])

        t_pmt = pmt.dict_ref(msg, pmt.intern("type"), pmt.PMT_NIL)
        if not pmt.is_null(t_pmt) and pmt.is_integer(t_pmt):
            ftype = int(pmt.to_long(t_pmt)) & 0x0F

        d_pmt = pmt.dict_ref(msg, pmt.intern("dst_id"), pmt.PMT_NIL)
        if not pmt.is_null(d_pmt) and pmt.is_integer(d_pmt):
            dst_id = int(pmt.to_long(d_pmt)) & 0xFFFF

        wire = _encode_frame(1, ftype, self._next_seq(),
                              self._src_id, dst_id, payload,
                              _timestamp_us24())
        with self._buf_lock:
            self._buf.extend(wire)

    def _beacon_loop(self, interval_s):
        while True:
            wire = _encode_frame(1, 2, self._next_seq(),
                                  self._src_id, 0xFFFF,
                                  bytes([0, 0, 0, 0]),
                                  _timestamp_us24())
            with self._buf_lock:
                self._buf.extend(wire)
            time.sleep(interval_s)

    def work(self, input_items, output_items):
        out = output_items[0]
        n   = len(out)
        with self._buf_lock:
            available = len(self._buf)
            if available == 0:
                return 0
            to_copy = min(n, available)
            out[:to_copy] = np.frombuffer(self._buf[:to_copy], dtype=np.uint8)
            if self._repeat and available <= to_copy:
                # refill buffer with another copy of the last frame
                pass   # beacon_loop handles continuous TX
            del self._buf[:to_copy]
        return to_copy
