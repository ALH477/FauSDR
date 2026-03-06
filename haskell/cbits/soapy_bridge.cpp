/*
 * soapy_bridge.cpp — SoapySDR C++ API wrapped in C-callable functions
 * DeMoD LLC
 */

#include "soapy_bridge.h"

#include <SoapySDR/Device.hpp>
#include <SoapySDR/Formats.hpp>
#include <SoapySDR/Logger.hpp>

#include <cstdio>
#include <cstring>

extern "C" {

SoapyDevHandle soapy_open(const char* driver) {
    SoapySDR::Kwargs args;
    if (driver && driver[0]) args["driver"] = driver;
    try {
        SoapySDR::Device* dev = SoapySDR::Device::make(args);
        return static_cast<SoapyDevHandle>(dev);
    } catch (...) {
        fprintf(stderr, "[soapy-bridge] Device::make failed\n");
        return nullptr;
    }
}

void soapy_configure_tx(SoapyDevHandle handle,
                          double center_freq_hz,
                          double sample_rate_hz,
                          double gain_db)
{
    auto* dev = static_cast<SoapySDR::Device*>(handle);
    dev->setSampleRate(SOAPY_SDR_TX, 0, sample_rate_hz);
    dev->setFrequency  (SOAPY_SDR_TX, 0, center_freq_hz);
    dev->setGain       (SOAPY_SDR_TX, 0, gain_db);
}

SoapyStreamHandle soapy_open_tx_stream(SoapyDevHandle handle) {
    auto* dev = static_cast<SoapySDR::Device*>(handle);
    try {
        SoapySDR::Stream* stream = dev->setupStream(SOAPY_SDR_TX, SOAPY_SDR_CF32);
        dev->activateStream(stream);
        return static_cast<SoapyStreamHandle>(stream);
    } catch (...) {
        fprintf(stderr, "[soapy-bridge] setupStream failed\n");
        return nullptr;
    }
}

int soapy_write_cf32(SoapyDevHandle    devHandle,
                      SoapyStreamHandle streamHandle,
                      const float*      iq_interleaved,
                      int               num_samples,
                      long              timeout_us)
{
    auto* dev    = static_cast<SoapySDR::Device*>(devHandle);
    auto* stream = static_cast<SoapySDR::Stream*>(streamHandle);

    /* SoapySDR expects an array of channel buffer pointers.
     * CF32 interleaved means each pair [I,Q] is one complex sample.
     * We pass a single pointer to the already-interleaved buffer. */
    const void* bufs[1] = { iq_interleaved };
    int flags   = 0;
    long long t = 0;

    return dev->writeStream(stream, bufs,
                            static_cast<size_t>(num_samples),
                            flags, t, timeout_us);
}

void soapy_close_stream(SoapyDevHandle devHandle, SoapyStreamHandle streamHandle) {
    auto* dev    = static_cast<SoapySDR::Device*>(devHandle);
    auto* stream = static_cast<SoapySDR::Stream*>(streamHandle);
    dev->deactivateStream(stream);
    dev->closeStream(stream);
}

void soapy_close(SoapyDevHandle handle) {
    SoapySDR::Device::unmake(static_cast<SoapySDR::Device*>(handle));
}

} // extern "C"
