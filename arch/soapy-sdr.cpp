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

/* htons/htonl for portable big-endian serialisation */
#ifdef _WIN32
#  include <winsock2.h>
#else
#  include <arpa/inet.h>
#endif

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
        // Sequence number: stored big-endian on the wire (matches dcf_frame.h)
        frame.seq = htons(seq++);
        // timestamp: microseconds since start (24-bit, wraps ~16s)
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count() & 0xFFFFFF;
        frame.timestamp[0] = (us >> 16) & 0xFF;
        frame.timestamp[1] = (us >>  8) & 0xFF;
        frame.timestamp[2] =  us        & 0xFF;
        frame_seal(frame);

        // Encode the sealed 17-byte frame as a BPSK NRZ symbol stream and
        // feed it into Faust channel 0 (the symbol input).
        //
        // Mapping: bit=1 → +1.0f, bit=0 → -1.0f (NRZ-L)
        // Each bit is held for samplesPerSymbol samples (rectangular pulse);
        // the Faust DSP then applies RRC pulse shaping and phase-modulates
        // onto the carrier.
        //
        // samplesPerSymbol = floor(sampleRate / symbolRate)
        //   = floor(2_000_000 / 9_600) = 208 samples/symbol
        // totalSymbolSamples = 136 bits × 208 = 28,288 samples/frame
        //
        // The Faust block size (burstSize) is typically 1024. We interleave
        // frame encoding with Faust compute blocks here.
        {
            static constexpr int SYMBOL_RATE   = 9600;
            const int sps = static_cast<int>(sampleRate) / SYMBOL_RATE;  // ≈208

            const uint8_t* frameBytes = reinterpret_cast<const uint8_t*>(&frame);
            // Flatten 17 bytes → 136 bits as symbol values ±1.0
            std::vector<float> symStream;
            symStream.reserve(136 * sps);
            for (int byte_i = 0; byte_i < 17; ++byte_i) {
                for (int bit_i = 7; bit_i >= 0; --bit_i) {
                    float sym = ((frameBytes[byte_i] >> bit_i) & 1) ? 1.0f : -1.0f;
                    for (int s = 0; s < sps; ++s)
                        symStream.push_back(sym);
                }
            }

            // Feed symbol stream to Faust in burstSize chunks, collect I/Q,
            // then write all chunks to the SDR in one pass.
            const size_t totalSamples = symStream.size();
            size_t offset = 0;
            while (offset < totalSamples) {
                size_t chunkLen = std::min<size_t>(burstSize, totalSamples - offset);
                // Zero-pad to burstSize so Faust always sees a full block
                std::fill(iBuf.begin(), iBuf.end(), 0.0f);
                std::copy(symStream.begin() + offset,
                          symStream.begin() + offset + chunkLen,
                          iBuf.begin());
                FAUSTFLOAT* inputs[1]  = { iBuf.data() };   // channel 0 = symbol stream
                dsp.compute(static_cast<int>(burstSize), inputs, outputs);

                // Interleave I/Q → CF32, truncate to valid chunkLen samples
                iqInterleaved.resize(chunkLen);
                for (size_t n = 0; n < chunkLen; ++n)
                    iqInterleaved[n] = { outputs[0][n], outputs[1][n] };

                const void* txBufs[1] = { iqInterleaved.data() };
                int flags = 0; long long timeNs = 0;
                int ret2 = sdr->writeStream(txStream, txBufs, chunkLen,
                                            flags, timeNs, 1000000);
                if (ret2 < 0) {
                    std::cerr << "[demod-sdr] writeStream error: "
                              << SoapySDR::errToStr(ret2) << "\n";
                    goto teardown;
                }
                offset += chunkLen;
            }
        }

    }

    // -- Teardown -------------------------------------------------------------
teardown:
    sdr->deactivateStream(txStream);
    sdr->closeStream(txStream);
    SoapySDR::Device::unmake(sdr);
    std::cout << "[demod-sdr] TX stopped cleanly.\n";
    return 0;
}
