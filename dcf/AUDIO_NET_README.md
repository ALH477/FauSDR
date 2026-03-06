# AudioNet - Modern Dial-Up Audio Communication System

AudioNet is a nostalgic yet modern audio communication system that brings the classic dial-up modem experience to contemporary audio hardware. Built on top of the DCF Faust SDR framework, it provides variable-rate BPSK modulation with authentic modem sounds for mono and stereo music equipment.

## Overview

AudioNet transforms your audio interface into a modern dial-up modem, complete with:
- **Variable symbol rates** from 1200 to 38400 baud
- **Audible carrier frequencies** (1-4kHz range) for that classic modem sound
- **Mono and stereo operation** for different hardware setups
- **Nostalgic connection simulation** with dialing, handshake, and disconnect sounds
- **Professional audio quality** up to 192kHz sample rates

## System Architecture

### Modulator Variants

AudioNet provides four specialized modulators:

1. **`modulator_audionet.dsp`** - Main variable-rate modulator with stereo support
2. **`modulator_audionet_mono.dsp`** - Optimized for mono audio hardware
3. **`modulator_audionet_stereo.dsp`** - Dual-channel for maximum throughput
4. **`modulator_audionet_legacy.dsp`** - Pure nostalgic dial-up experience

### Key Features

- **Variable Symbol Rates**: 1200, 2400, 9600, 19200, 38400 baud
- **Audible Carrier Frequencies**: 1-4kHz range for classic modem sounds
- **Stereo Operation**: Dual channels for 2x throughput or redundancy
- **Connection Simulation**: Full dial-up experience with state transitions
- **Professional Audio**: Support for 44.1kHz to 192kHz sample rates
- **Configurable Profiles**: Presets for different use cases and sound profiles

## Performance Specifications

### Data Throughput

| Mode | Symbol Rate | Data Rate | Latency | Range |
|------|-------------|-----------|---------|-------|
| Mono | 1.2-19.2 kbps | 1.2-19.2 kbps | 5-20ms | 10-100m |
| Stereo | 2.4-38.4 kbps | 2.4-38.4 kbps | 3-15ms | 5-50m |

### Audio Quality Levels

| Quality | Sample Rate | Use Case |
|---------|-------------|----------|
| Standard | 44.1kHz | Basic audio equipment |
| High | 48kHz-96kHz | Professional audio interfaces |
| Studio | 192kHz | High-end studio equipment |

## Configuration

### Basic Configuration

```json
{
  "modulator_type": "audionet",
  "audio_mode": "stereo",
  "sample_rate": 48000,
  "carrier_frequency": {
    "primary": 2200,
    "secondary": 2600
  },
  "symbol_rate": {
    "primary": 9600,
    "secondary": 4800
  },
  "modulation": {
    "modulation_index": 3.14159,
    "rrc_rolloff": 0.35,
    "output_gain": 0.8
  }
}
```

### Sound Profiles

- **`deep_bbs`**: 1.8kHz carrier, slow 1200 baud - Teletype experience
- **`classic_modem`**: 2.2kHz carrier, 2400 baud - Classic dial-up
- **`high_speed`**: 2.8kHz carrier, 9600 baud - Fast modem sounds
- **`ultra_clear`**: 3.5kHz carrier, 19200+ baud - Crystal clear

### Connection States

1. **Idle**: Waiting for connection
2. **Dialing**: 1.2kHz rising tone (0.5s)
3. **Handshake**: Alternating 2.2kHz/1.8kHz beeps (2s)
4. **Connected**: Data transmission with modem screech
5. **Disconnecting**: Descending 3kHz to 1kHz tone (0.3s)

## Hardware Requirements

### Minimum Requirements
- Audio interface with line-level I/O
- Sample rate support: 44.1kHz or higher
- Full duplex operation
- 16-bit or higher resolution

### Recommended Setup
- Professional audio interface (48kHz-192kHz)
- Balanced line connections
- Low-latency drivers (ASIO, Core Audio, JACK)
- Studio-quality cabling

### Stereo Setup
- Dual-channel audio interface
- Separate left/right channel connections
- Independent gain control per channel
- Phase-coherent cabling

## Installation

### Prerequisites
- Faust compiler (version 2.50.0 or later)
- Haskell development environment
- SoapySDR (for SDR operation)
- Audio interface drivers

### Build Instructions

1. **Compile Faust DSP files:**
   ```bash
   faust -a faust_bridge.cpp dsp/modulator_audionet.dsp -o dsp/c_sdk/modulator_audionet.cpp
   faust -a faust_bridge.cpp dsp/modulator_audionet_mono.dsp -o dsp/c_sdk/modulator_audionet_mono.cpp
   faust -a faust_bridge.cpp dsp/modulator_audionet_stereo.dsp -o dsp/c_sdk/modulator_audionet_stereo.cpp
   faust -a faust_bridge.cpp dsp/modulator_audionet_legacy.dsp -o dsp/c_sdk/modulator_audionet_legacy.cpp
   ```

2. **Build Haskell transport layer:**
   ```bash
   cd haskell
   cabal build
   ```

3. **Configure audio interface:**
   - Set sample rate to desired value (48kHz recommended)
   - Configure buffer size for low latency (512 samples)
   - Enable full duplex mode

## Usage Examples

### Basic Mono Operation
```bash
# Start with mono configuration
./dcf-faust-sdr --config config-mono.json
```

### High-Speed Stereo
```bash
# Start with stereo configuration
./dcf-faust-sdr --config config-stereo.json
```

### Legacy Dial-Up Experience
```bash
# Start with nostalgic configuration
./dcf-faust-sdr --config config-legacy.json
```

## Use Cases

### Music Production
- Studio-to-studio file transfer over audio lines
- Live performance data synchronization
- Equipment control via audio signals

### Gaming/VR
- Low-latency controller communication
- Audio-based positional data transmission
- Multi-room synchronization

### IoT/Home Automation
- Audio-based sensor networks
- Smart home device communication
- Backup communication channel

### Educational
- Digital communications demonstration
- Modulation technique examples
- Audio signal processing education

## Troubleshooting

### Common Issues

**No Audio Output:**
- Check audio interface configuration
- Verify sample rate compatibility
- Ensure full duplex mode is enabled

**Poor Connection Quality:**
- Reduce symbol rate for noisy environments
- Increase output gain (but avoid clipping)
- Use shorter, higher-quality cables

**High Latency:**
- Reduce audio buffer size
- Use ASIO/Core Audio/JACK drivers
- Close other audio applications

**Stereo Channel Issues:**
- Verify independent channel configuration
- Check phase coherence of cables
- Ensure balanced connections

### Performance Optimization

**For Maximum Throughput:**
- Use 192kHz sample rate
- Enable stereo mode with independent channels
- Set symbol rates to 19200+ baud
- Use low-latency audio drivers

**For Maximum Range:**
- Use lower symbol rates (1200-2400 baud)
- Increase output gain (avoid clipping)
- Use shielded, high-quality cables
- Consider audio amplifiers for long runs

**For Lowest Latency:**
- Use 64-128 sample buffers
- Enable realtime CPU priority
- Use direct audio interface connections
- Disable audio processing effects

## Development

### Adding New Sound Profiles
1. Modify the carrier frequency and symbol rate presets
2. Add new button definitions in the DSP file
3. Update the configuration schema
4. Test with different audio hardware

### Custom Modulation
1. Create new DSP file based on existing templates
2. Implement custom modulation scheme
3. Update transport layer for new parameters
4. Add configuration options

### Hardware Integration
1. Test with different audio interfaces
2. Optimize for specific hardware characteristics
3. Create hardware-specific configuration profiles
4. Document compatibility and limitations

## License

This project is licensed under the same license as the parent DCF Faust SDR project.

## Contributing

Contributions are welcome! Please follow these guidelines:
1. Test thoroughly with different audio hardware
2. Maintain backward compatibility
3. Update documentation for new features
4. Provide configuration examples

## Support

For support and questions:
- Check the troubleshooting section
- Review configuration examples
- Test with different audio hardware
- Consult the parent DCF Faust SDR documentation