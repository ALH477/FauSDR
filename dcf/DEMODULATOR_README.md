# AudioNet Demodulators - Complete Guide

This document provides comprehensive information about the AudioNet demodulator family, designed to perfectly match each AudioNet modulator variant.

## Overview

The AudioNet demodulator family consists of four specialized demodulators that provide optimal performance for different use cases:

1. **`demodulator_audionet.dsp`** - Main variable-rate demodulator with adaptive features
2. **`demodulator_audionet_mono.dsp`** - Simplified mono audio demodulator
3. **`demodulator_audionet_stereo.dsp`** - Dual-channel stereo demodulator with diversity combining
4. **`demodulator_audionet_legacy.dsp`** - Nostalgic fixed-rate demodulator with connection simulation

## Technical Specifications

### Core Demodulation Technology

All demodulators implement:
- **Coherent BPSK detection** with carrier recovery
- **Matched filtering** using RRC pulse shaping
- **Symbol timing recovery** for synchronization
- **Variable rate adaptation** to match modulator rates
- **Audible carrier frequency tracking** (1-4kHz range)

### Performance Specifications

| Demodulator | Symbol Rate Range | Carrier Freq Range | Latency | Throughput |
|-------------|------------------|-------------------|---------|------------|
| Main | 1200-38400 baud | 1-4kHz | 2-10ms | 1.2-38.4 kbps |
| Mono | 1200-19200 baud | 1-4kHz | 3-15ms | 1.2-19.2 kbps |
| Stereo | 1200-38400 baud | 1-4kHz | 2-8ms | 2.4-38.4 kbps |
| Legacy | Fixed rates | Fixed freqs | 5-20ms | 0.3-9.6 kbps |

## Individual Demodulator Details

### 1. Main Variable-Rate Demodulator (`demodulator_audionet.dsp`)

**Key Features:**
- Adaptive carrier recovery with PLL
- Gardner timing error detection
- Variable symbol rate tracking
- Real-time rate negotiation
- Connection state detection

**Configuration Parameters:**
```json
{
  "demodulator_type": "audionet",
  "expected_carrier": 2200,
  "expected_symbol_rate": 9600,
  "pll_bandwidth": 0.01,
  "timing_gain": 0.1,
  "rrc_rolloff": 0.35
}
```

**Use Cases:**
- Professional audio applications
- Variable rate communication
- High-performance data transmission
- Adaptive environments

### 2. Mono Audio Demodulator (`demodulator_audionet_mono.dsp`)

**Key Features:**
- Fixed carrier frequency tracking
- Simplified timing recovery
- Lower computational overhead
- Optimized for basic audio interfaces

**Configuration Parameters:**
```json
{
  "demodulator_type": "audionet_mono",
  "carrier_freq": 2200,
  "symbol_rate": 9600,
  "pll_bandwidth": 0.02,
  "timing_gain": 0.2
}
```

**Use Cases:**
- Basic audio equipment
- Low-latency applications
- Embedded systems
- Educational demonstrations

### 3. Stereo Dual-Channel Demodulator (`demodulator_audionet_stereo.dsp`)

**Key Features:**
- Independent left/right channel demodulation
- Channel diversity combining (selection, equal gain, maximal ratio)
- Higher throughput with parallel processing
- Channel quality monitoring
- Error detection and correction

**Configuration Parameters:**
```json
{
  "demodulator_type": "audionet_stereo",
  "carrier_freq_left": 2200,
  "carrier_freq_right": 2600,
  "symbol_rate_left": 9600,
  "symbol_rate_right": 4800,
  "diversity_mode": 2,
  "output_gain_left": 1.0,
  "output_gain_right": 1.0
}
```

**Diversity Combining Modes:**
- **Mode 0**: Selection combining (best channel)
- **Mode 1**: Equal gain combining
- **Mode 2**: Maximal ratio combining (optimal SNR)

**Use Cases:**
- High-throughput applications
- Redundant communication
- Professional studio environments
- Long-distance audio transmission

### 4. Legacy Dial-Up Demodulator (`demodulator_audionet_legacy.dsp`)

**Key Features:**
- Fixed rate profiles (300, 1200, 2400, 9600 baud)
- Authentic connection state simulation
- Dial-up sound effect generation
- Period-accurate timing
- Backward compatibility

**Configuration Parameters:**
```json
{
  "demodulator_type": "audionet_legacy",
  "sound_profile": 2,
  "connection_state": 0,
  "output_gain": 1.0
}
```

**Sound Profiles:**
- **Profile 0**: 300 baud Teletype (1.8kHz)
- **Profile 1**: 1200 baud Early Modem (2.2kHz)
- **Profile 2**: 2400 baud Classic Modem (2.4kHz)
- **Profile 3**: 9600 baud Fast Modem (2.8kHz)

**Connection States:**
1. **Idle**: Waiting for connection
2. **Dialing**: Detect 1.2kHz dial tone
3. **Handshake**: Detect alternating beeps
4. **Connected**: Normal BPSK demodulation
5. **Disconnecting**: Detect descending tone

**Use Cases:**
- Nostalgic dial-up experience
- Educational demonstrations
- Period-accurate simulations
- Artistic installations

## Integration with Transport Layer

### Configuration Schema Extension

The demodulator configuration extends the main AudioNet schema:

```json
{
  "demodulator": {
    "type": "audionet|audionet_mono|audionet_stereo|audionet_legacy",
    "parameters": {
      // Demodulator-specific parameters
    }
  }
}
```

### Auto-Configuration

The system automatically:
- Detects modulator type and selects matching demodulator
- Configures parameters based on modulator settings
- Optimizes performance for the specific hardware setup
- Provides fallback configurations for compatibility

### Manual Override

Users can manually specify demodulator settings:
```json
{
  "modulator_type": "audionet_stereo",
  "demodulator_type": "audionet_stereo",
  "demodulator_config": {
    "diversity_mode": 2,
    "carrier_freq_left": 2200,
    "carrier_freq_right": 2600
  }
}
```

## Performance Optimization

### For Maximum Throughput
- Use stereo demodulator with maximal ratio combining
- Set symbol rates to 19200+ baud
- Use 192kHz sample rate
- Enable low-latency audio drivers

### For Maximum Range
- Use mono demodulator with lower symbol rates
- Increase PLL bandwidth for better tracking
- Use higher output gains (avoid clipping)
- Implement error correction protocols

### For Lowest Latency
- Use simplified mono demodulator
- Reduce audio buffer sizes
- Disable diversity combining
- Use direct audio interface connections

### For Best Audio Quality
- Use 96kHz or 192kHz sample rates
- Enable RRC filtering with optimal rolloff
- Use professional audio interfaces
- Implement proper gain staging

## Troubleshooting

### Common Issues

**No Signal Detected:**
- Check audio interface configuration
- Verify sample rate compatibility
- Ensure proper gain levels
- Check cable connections

**Poor Connection Quality:**
- Reduce symbol rate for noisy environments
- Adjust PLL bandwidth for better tracking
- Increase RRC rolloff for better filtering
- Use diversity combining in stereo mode

**High Error Rate:**
- Check SNR and adjust gains
- Verify carrier frequency accuracy
- Use error correction protocols
- Implement adaptive rate control

**Stereo Channel Issues:**
- Verify independent channel configuration
- Check phase coherence
- Ensure balanced connections
- Monitor channel quality indicators

## Development and Customization

### Adding New Demodulator Features
1. Create new DSP file based on existing templates
2. Implement custom algorithms in Faust
3. Update configuration schema
4. Add parameter controls
5. Test with different audio hardware

### Custom Modulation Schemes
1. Modify carrier recovery algorithms
2. Implement new timing recovery methods
3. Add support for different modulation types
4. Update matched filtering for new schemes
5. Create appropriate configuration options

### Hardware-Specific Optimization
1. Test with target audio interfaces
2. Optimize for specific sample rates
3. Tune parameters for hardware characteristics
4. Create hardware-specific profiles
5. Document compatibility and limitations

## Compatibility Matrix

| Modulator | Compatible Demodulators | Notes |
|-----------|------------------------|-------|
| `modulator_audionet` | All | Best with main demodulator |
| `modulator_audionet_mono` | Mono, Main | Mono optimized for basic hardware |
| `modulator_audionet_stereo` | Stereo, Main | Stereo provides best performance |
| `modulator_audionet_legacy` | Legacy, Main | Legacy provides authentic experience |

## Future Enhancements

### Planned Features
- **Adaptive equalization** for long cable runs
- **Multi-path mitigation** for complex environments
- **Machine learning** for signal classification
- **Real-time spectrum analysis** for monitoring
- **Advanced error correction** protocols

### Research Areas
- **Deep learning** for signal enhancement
- **Cognitive radio** techniques for spectrum efficiency
- **Quantum-inspired** algorithms for optimization
- **Neuromorphic** processing for low power
- **Edge computing** for distributed processing

This comprehensive demodulator family provides the perfect foundation for modern dial-up audio communication, offering everything from nostalgic experiences to cutting-edge performance.