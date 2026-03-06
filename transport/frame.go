// Package frame implements the DeMoD 17-byte DCF transport frame codec.
//
// Wire layout:
//
//	Byte  Field       Notes
//	────  ─────────   ──────────────────────────────────────────────
//	 0    sync        Fixed 0xD3
//	 1    flags       [7:4] version (4-bit) | [3:0] type (4-bit)
//	 2-3  seq         Big-endian uint16
//	 4-5  src_id      Big-endian uint16
//	 6-7  dst_id      Big-endian uint16  (0xFFFF = broadcast)
//	 8-11 payload     4 raw bytes
//	12-14 timestamp   24-bit big-endian µs offset (wraps ~16.7 s)
//	15-16 crc16       CRC-CCITT(0x1021, init=0xFFFF) over bytes [0..14]
//
// Reference test vector:
//
//	Input:  version=1, DATA, seq=1, src=1, dst=0xFFFF, payload=DEADBEEF, ts=0
//	Output: D3 10 00 01 00 01 FF FF DE AD BE EF 00 00 00 42 DD
//
// The CRC value 0x42DD is pinned across all language implementations.
// If this package's TestCRCPin fails, the codec has diverged from the others.
//
// No external dependencies.
package frame

import "errors"

// FrameSize is the fixed wire size in bytes.
const FrameSize = 17

// SyncByte is the fixed preamble byte.
const SyncByte = 0xD3

// Broadcast is the special dst_id meaning "all nodes".
const Broadcast uint16 = 0xFFFF

// Type represents the 4-bit frame type nibble.
type Type uint8

const (
	TypeData   Type = 0 // application data
	TypeAck    Type = 1 // acknowledgement
	TypeBeacon Type = 2 // clock sync / beacon
	TypeCtrl   Type = 3 // control / fragmentation
)

func (t Type) String() string {
	switch t {
	case TypeData:
		return "DATA"
	case TypeAck:
		return "ACK"
	case TypeBeacon:
		return "BEACON"
	case TypeCtrl:
		return "CTRL"
	default:
		return "UNKNOWN"
	}
}

// Sentinel errors returned by Decode.
var (
	ErrBadLength  = errors.New("frame: buffer must be exactly 17 bytes")
	ErrBadSync    = errors.New("frame: invalid sync byte (expected 0xD3)")
	ErrBadCRC     = errors.New("frame: CRC mismatch")
	ErrUnknownType = errors.New("frame: unknown frame type nibble")
)

// Frame is the decoded representation of a DCF transport frame.
type Frame struct {
	Version     uint8    // 4-bit (0–15)
	Type        Type     // 4-bit
	Seq         uint16   // big-endian rolling counter
	SrcID       uint16
	DstID       uint16
	Payload     [4]byte
	TimestampUs uint32   // lower 24 bits used; upper byte ignored on encode
}

// ── CRC-CCITT (poly 0x1021, init 0xFFFF) ─────────────────────────────────────
// Identical algorithm to C, Haskell, Rust, and Lisp implementations.

func crc16CCITT(data []byte) uint16 {
	crc := uint16(0xFFFF)
	for _, b := range data {
		crc ^= uint16(b) << 8
		for i := 0; i < 8; i++ {
			if crc&0x8000 != 0 {
				crc = (crc << 1) ^ 0x1021
			} else {
				crc <<= 1
			}
		}
	}
	return crc
}

// ── Encode ────────────────────────────────────────────────────────────────────

// Encode serialises f into exactly 17 bytes.
// CRC is computed and appended automatically.
// All multi-byte fields are big-endian.
func (f *Frame) Encode() [FrameSize]byte {
	var w [FrameSize]byte
	w[0] = SyncByte
	w[1] = ((f.Version & 0x0F) << 4) | (uint8(f.Type) & 0x0F)
	w[2] = uint8(f.Seq >> 8)
	w[3] = uint8(f.Seq)
	w[4] = uint8(f.SrcID >> 8)
	w[5] = uint8(f.SrcID)
	w[6] = uint8(f.DstID >> 8)
	w[7] = uint8(f.DstID)
	w[8]  = f.Payload[0]
	w[9]  = f.Payload[1]
	w[10] = f.Payload[2]
	w[11] = f.Payload[3]
	w[12] = uint8((f.TimestampUs >> 16) & 0xFF)
	w[13] = uint8((f.TimestampUs >>  8) & 0xFF)
	w[14] = uint8(f.TimestampUs & 0xFF)
	crc := crc16CCITT(w[:15])
	w[15] = uint8(crc >> 8)
	w[16] = uint8(crc)
	return w
}

// ── Decode ────────────────────────────────────────────────────────────────────

// Decode parses a 17-byte slice into a Frame.
// Returns ErrBadLength if len(b) != 17, ErrBadSync if b[0] != 0xD3,
// ErrBadCRC if the CRC fails, ErrUnknownType for a reserved type nibble.
func Decode(b []byte) (*Frame, error) {
	if len(b) != FrameSize {
		return nil, ErrBadLength
	}
	if b[0] != SyncByte {
		return nil, ErrBadSync
	}
	storedCRC := uint16(b[15])<<8 | uint16(b[16])
	if crc16CCITT(b[:15]) != storedCRC {
		return nil, ErrBadCRC
	}

	typeNib := Type(b[1] & 0x0F)
	if typeNib > TypeCtrl {
		return nil, ErrUnknownType
	}

	f := &Frame{
		Version: (b[1] >> 4) & 0x0F,
		Type:    typeNib,
		Seq:     uint16(b[2])<<8 | uint16(b[3]),
		SrcID:   uint16(b[4])<<8 | uint16(b[5]),
		DstID:   uint16(b[6])<<8 | uint16(b[7]),
		Payload: [4]byte{b[8], b[9], b[10], b[11]},
		TimestampUs: uint32(b[12])<<16 | uint32(b[13])<<8 | uint32(b[14]),
	}
	return f, nil
}

// ── IsValid ───────────────────────────────────────────────────────────────────

// IsValid returns true iff b is a valid 17-byte frame (sync + CRC correct).
// Does not fully decode; cheaper than Decode for hot-path filtering.
func IsValid(b []byte) bool {
	if len(b) != FrameSize || b[0] != SyncByte {
		return false
	}
	stored := uint16(b[15])<<8 | uint16(b[16])
	return crc16CCITT(b[:15]) == stored
}
