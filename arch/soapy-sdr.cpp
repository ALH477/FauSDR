/*
 * soapy-sdr.cpp  —  DeMoD Faust Architecture for SoapySDR IQ Output
 *
 * Usage:
 *   faust -a arch/soapy-sdr.cpp -lang c++ -vec -vs 256 dsp/modulator.dsp -o build/modulator.cpp
 *   clang++ build/modulator.cpp -o build/modulator -lSoapySDR -lm
 *
 * Convention:
 *   Faust channel 0  →  I (in-phase)
 *   Faust channel 1  →  Q (quadrature)
 *
 * The Faust DSP program receives no audio inputs (tx-only) and produces
 * exactly 2 output channels (I, Q) at the configured sample rate.
 *
 * DeMoD LLC — Custom transport: 17-byte frame format
 */

#include <SoapySDR/Device.hpp>
#include <SoapySDR/Formats.hpp>
#include <SoapySDR/Logger.hpp>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <complex>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <string>
#include <vector>

// ── Faust-generated DSP (injected by compiler) ────────────────────────────────
<<includeIntrinsic>>
<<includeclass>>

// ── Runtime parameters (override via env or CLI) ──────────────────────────────
static const double  DEFAULT_SAMPLE_RATE  = 2e6;    // 2 MSPS
static const double  DEFAULT_CENTER_FREQ  = 433.92e6;
static const double  DEFAULT_TX_GAIN      = 20.0;   // dB
static const size_t  DEFAULT_BURST_SIZE   = 1024;   // samples per SoapySDR write
static const std::string DEFAULT_DRIVER   = "rtlsdr"; // change to "hackrf", "lime", etc.

// ── 17-byte transport frame ───────────────────────────────────────────────────
// Frame layout (17 bytes = 136 bits):
//   [0]     SYNC       0xD3                    (1 byte,  fixed preamble)
//   [1]     FLAGS      [7:4]=version [3:0]=type (1 byte)
//   [2-3]   SEQ        big-endian uint16        (2 bytes, rolling counter)
//   [4-5]   SRC_ID     source node ID           (2 bytes)
//   [6-7]   DST_ID     destination node ID      (2 bytes)
//   [8-11]  PAYLOAD    application data         (4 bytes)
//   [12-14] TIMESTAMP  24-bit microsec offset   (3 bytes)
//   [15-16] CRC16      CRC-CCITT of bytes 0-14  (2 bytes)
#pragma pack(push, 1)
struct DeModFrame {
    uint8_t  sync;          // 0xD3
    uint8_t  flags;         // [7:4] version, [3:0] frame_type
    uint16_t seq;           // big-endian rolling sequence number
    uint16_t src_id;        // source node
    uint16_t dst_id;        // destination node
    uint8_t  payload[4];    // application payload
    uint8_t  timestamp[3];  // 24-bit µs offset (big-endian)
    uint16_t crc16;         // CRC-CCITT(0xFFFF) of bytes [0..14]
};
#pragma pack(pop)
static_assert(sizeof(DeModFrame) == 17, "Frame must be exactly 17 bytes");

// CRC-CCITT (poly 0x1021, init 0xFFFF)
static uint16_t crc16_ccitt(const uint8_t* data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
    }
    return crc;
}

static void frame_seal(DeModFrame& f) {
    f.sync  = 0xD3;
    f.crc16 = crc16_ccitt(reinterpret_cast<const uint8_t*>(&f), 15);
}

static bool frame_validate(const DeModFrame& f) {
    if (f.sync != 0xD3) return false;
    return crc16_ccitt(reinterpret_cast<const uint8_t*>(&f), 15) == f.crc16;
}

// ── Signal handler ────────────────────────────────────────────────────────────
static std::atomic<bool> gRunning{true};
static void sigHandler(int) { gRunning.store(false); }

// ── Arch entrypoint ───────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    signal(SIGINT,  sigHandler);
    signal(SIGTERM, sigHandler);

    // -- Parameters (extend with getopt/argparse as needed) -------------------
    const double      sampleRate  = DEFAULT_SAMPLE_RATE;
    const double      centerFreq  = DEFAULT_CENTER_FREQ;
    const double      txGain      = DEFAULT_TX_GAIN;
    const size_t      burstSize   = DEFAULT_BURST_SIZE;
    const std::string driver      = DEFAULT_DRIVER;

    std::cout << "[demod-sdr] driver=" << driver
              << " freq=" << centerFreq/1e6 << " MHz"
              << " rate=" << sampleRate/1e6 << " MSPS"
              << " gain=" << txGain << " dB\n";

    // -- Open SoapySDR device --------------------------------------------------
    SoapySDR::Kwargs args = {{"driver", driver}};
    SoapySDR::Device* sdr = SoapySDR::Device::make(args);
    if (!sdr) throw std::runtime_error("SoapySDR: device not found");

    sdr->setSampleRate(SOAPY_SDR_TX, 0, sampleRate);
    sdr->setFrequency  (SOAPY_SDR_TX, 0, centerFreq);
    sdr->setGain       (SOAPY_SDR_TX, 0, txGain);

    // -- TX stream (native complex float CF32) --------------------------------
    SoapySDR::Stream* txStream = sdr->setupStream(SOAPY_SDR_TX, SOAPY_SDR_CF32);
    sdr->activateStream(txStream);

    // -- Faust DSP instance ----------------------------------------------------
    mydsp dsp;
    dsp.init(static_cast<int>(sampleRate));
    assert(dsp.getNumOutputs() == 2 && "Faust program must produce exactly 2 channels (I, Q)");

    // -- Buffers ---------------------------------------------------------------
    std::vector<float> iBuf(burstSize), qBuf(burstSize);
    std::vector<std::complex<float>> iqInterleaved(burstSize);
    FAUSTFLOAT* outputs[2] = { iBuf.data(), qBuf.data() };

    // -- Transport frame state ------------------------------------------------
    DeModFrame frame{};
    frame.flags  = (0x1 << 4) | 0x0; // version=1, type=DATA
    frame.src_id = 0x0001;
    frame.dst_id = 0xFFFF;           // broadcast
    uint16_t seq = 0;

    std::cout << "[demod-sdr] TX running — Ctrl+C to stop\n";

    // -- Main loop ------------------------------------------------------------
    while (gRunning.load()) {
        // Build & seal the transport frame for this burst
        frame.seq = __builtin_bswap16(seq++);
        // timestamp: microseconds since start (24-bit, wraps ~16s)
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count() & 0xFFFFFF;
        frame.timestamp[0] = (us >> 16) & 0xFF;
        frame.timestamp[1] = (us >>  8) & 0xFF;
        frame.timestamp[2] =  us        & 0xFF;
        frame_seal(frame);

        // TODO: encode frame bytes into payload signal that Faust receives
        // (implement your symbol mapper here, e.g. BPSK/QPSK/custom)
        // For now, Faust generates the waveform autonomously (e.g. test tone).

        // Run Faust compute
        dsp.compute(static_cast<int>(burstSize), nullptr, outputs);

        // Interleave I/Q → CF32
        for (size_t n = 0; n < burstSize; n++)
            iqInterleaved[n] = { iBuf[n], qBuf[n] };

        // Write to SDR
        const void* txBufs[1] = { iqInterleaved.data() };
        int flags = 0;
        long long timeNs = 0;
        int ret = sdr->writeStream(txStream, txBufs,
                                   static_cast<size_t>(burstSize),
                                   flags, timeNs, 1000000 /*1s timeout*/);
        if (ret < 0) {
            std::cerr << "[demod-sdr] writeStream error: " << SoapySDR::errToStr(ret) << "\n";
            break;
        }
    }

    // -- Teardown -------------------------------------------------------------
    sdr->deactivateStream(txStream);
    sdr->closeStream(txStream);
    SoapySDR::Device::unmake(sdr);
    std::cout << "[demod-sdr] TX stopped cleanly.\n";
    return 0;
}
