/*
 * soapy_bridge.h — C-callable wrapper for SoapySDR C++ API
 *
 * Exposes the minimum SoapySDR surface needed for a TX-only IQ stream
 * to Haskell via foreign import ccall.
 *
 * DeMoD LLC
 */
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* SoapyDevHandle;
typedef void* SoapyStreamHandle;

/**
 * Open an SDR device by driver name (e.g. "rtlsdr", "hackrf", "lime", "plutosdr").
 * Returns NULL on failure.
 */
SoapyDevHandle soapy_open(const char* driver);

/**
 * Configure TX channel 0.
 * @param center_freq_hz  carrier frequency in Hz (e.g. 433.92e6)
 * @param sample_rate_hz  sample rate in Hz (e.g. 2e6)
 * @param gain_db         TX gain in dB
 */
void soapy_configure_tx(SoapyDevHandle dev,
                         double center_freq_hz,
                         double sample_rate_hz,
                         double gain_db);

/**
 * Open and activate a CF32 TX stream on channel 0.
 * Returns NULL on failure.
 */
SoapyStreamHandle soapy_open_tx_stream(SoapyDevHandle dev);

/**
 * Write interleaved complex float (CF32) samples to the TX stream.
 *
 * @param iq_interleaved  buffer of [I0, Q0, I1, Q1, ...] float pairs
 * @param num_samples     number of IQ pairs (buffer length = num_samples * 2 floats)
 * @param timeout_us      write timeout in microseconds
 * @return                samples written (>0) or negative SoapySDR error code
 */
int soapy_write_cf32(SoapyDevHandle   dev,
                      SoapyStreamHandle stream,
                      const float*     iq_interleaved,
                      int              num_samples,
                      long             timeout_us);

/** Deactivate and close the TX stream. */
void soapy_close_stream(SoapyDevHandle dev, SoapyStreamHandle stream);

/** Close and free the SDR device. */
void soapy_close(SoapyDevHandle dev);

#ifdef __cplusplus
}
#endif
