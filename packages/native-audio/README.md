# @potrepka/native-audio

High-performance audio DSP library for TypeScript with Web (WASM) and React Native (JSI) support.

## Features

- 🚀 **60+ DSP Nodes** - Complete audio processing toolkit
- 🎯 **Node Graph System** - Connect nodes like a modular synthesizer
- 🔊 **Multi-Channel Audio** - Process mono, stereo, or multi-channel audio
- ⚡ **High Performance** - Native C++ processing via WASM/JSI
- 🌐 **Cross-Platform** - Works on Web and React Native
- 📱 **Zero Dependencies** - Self-contained audio processing

## Installation

```bash
npm install @potrepka/native-audio
```

### React Native Setup

This library requires React Native 0.70+ with the new architecture enabled (JSI).

**iOS:**

```bash
cd ios && pod install
```

**Android:**
No additional setup required.

## Quick Start

```typescript
import { Biquad, FilterType } from '@potrepka/native-audio'

// Create a lowpass filter
const filter = new Biquad(2, 44100, FilterType.LOWPASS)
filter.setFrequency(1000)
filter.setQ(1.0)

// Process audio
const inputs = [
  new Float32Array(512), // Left channel
  new Float32Array(512), // Right channel
]
const outputs = [new Float32Array(512), new Float32Array(512)]

filter.process(inputs, outputs, 512)

// Cleanup (required on Web, optional on Native)
filter.dispose()
```

## Node Categories

### Analyzers

- `Recorder` - Audio recording with fixed/variable buffer modes

### Channels

- `ChannelMerger` - Combine multiple channels
- `ChannelSplitter` - Split channels apart
- `MidSide` - Mid-side stereo processing
- `Spread` - Stereo width control
- `StereoPanner` - Pan positioning

### Delays

- `Convolver` - FFT-based convolution
- `VariableDelay` - Adjustable delay lines

### Dynamics

- `Clipper` - Hard/soft clipping
- `CompressorGate` - Compression and gating
- `DryWet` - Mix control
- `Envelope` - ADSR envelope follower
- `Lag` - Smoothing/slewing
- `Shaper` - Waveshaping/distortion

### Filters

- `Biquad` - 8 filter types (lowpass, highpass, bandpass, etc.)
- `Crossover` - Multi-band frequency splitting
- `OnePole` - Simple one-pole filter

### Generators

- `FunctionOscillator` - Custom function-based oscillator
- `MoorerOscillator` - Moorer synthesis
- `Noise` - Noise generation
- `Phasor` - Phase accumulator
- `SamplePlayer` - Sample playback
- `TableOscillator` - Wavetable synthesis

### Math Operations

- Basic: `Multiplication`, `Division`, `AbsoluteValue`, etc.
- FFT: `ForwardFFT`, `InverseFFT`
- Musical: `FrequencyToNote`, `NoteToFrequency`

### Triggers

- `ClockTrigger`, `ResetTrigger`, `SampleAndHold`, `Sequencer`, etc.

## Node Graph Example

```typescript
import {
  TableOscillator,
  Biquad,
  CompressorGate,
  VariableDelay,
  DryWet,
  FilterType,
} from '@potrepka/native-audio'

// Create nodes
const osc = new TableOscillator(1, 44100, 'sine')
const filter = new Biquad(1, 44100, FilterType.LOWPASS)
const compressor = new CompressorGate(1, 44100)
const delay = new VariableDelay(1, 44100, 0.5)
const dryWet = new DryWet(1)

// Connect nodes (chain)
osc.connect(filter)
filter.connect(compressor)
compressor.connect(delay)
delay.connect(dryWet)

// Configure parameters
osc.setFrequency(440)
filter.setFrequency(2000)
filter.setQ(1.0)
compressor.setThreshold(-20)
compressor.setRatio(4)
delay.setDelay(0.25)
dryWet.setWet(0.3)

// Process audio
const bufferSize = 512
const inputs = [new Float32Array(bufferSize)]
const outputs = [new Float32Array(bufferSize)]
dryWet.process(inputs, outputs, bufferSize)

// Cleanup
;[osc, filter, compressor, delay, dryWet].forEach((node) => node.dispose())
```

## Memory Management

### Web (WASM)

On Web, you **must** call `dispose()` when done with a node to prevent memory leaks:

```typescript
const filter = new Biquad(2, 44100)
try {
  filter.process(inputs, outputs, 512)
} finally {
  filter.dispose() // Required!
}
```

### React Native (JSI)

On React Native, memory is automatically managed by JavaScript's garbage collector. However, you can still call `dispose()` for immediate cleanup:

```typescript
const filter = new Biquad(2, 44100)
filter.process(inputs, outputs, 512)
filter.dispose() // Optional, but recommended
```

## Platform Detection

The library automatically detects the platform and uses the appropriate implementation:

```typescript
import { Platform } from '@potrepka/native-audio'

if (Platform.isWeb) {
  console.log('Running on Web with WASM')
} else if (Platform.isNative) {
  console.log('Running on React Native with JSI')
}
```

## API Documentation

### Node Base Class

All DSP nodes extend the `Node` base class:

```typescript
abstract class Node {
  connect(destination: Node): Node
  disconnect(destination?: Node): void
  dispose(): void
  abstract process(
    inputs: Array<Float32Array>,
    outputs: Array<Float32Array>,
    bufferSize: number,
  ): void
}
```

### Multi-Channel Processing

Audio is represented as an array of `Float32Array`, one per channel:

```typescript
// Mono (1 channel)
const mono = [new Float32Array(512)]

// Stereo (2 channels)
const stereo = [
  new Float32Array(512), // Left
  new Float32Array(512), // Right
]

// Surround (6 channels)
const surround = Array.from({ length: 6 }, () => new Float32Array(512))
```

## Performance Tips

1. **Use Larger Buffer Sizes** - Reduces overhead from buffer transfer
2. **Reuse Buffers** - Allocate buffers once and reuse them
3. **Dispose Nodes** - Call `dispose()` when done, especially on Web
4. **Batch Processing** - Process multiple buffers in a single call
5. **Profile First** - Use browser/React Native profiling tools before optimizing

## Build from Source

### Prerequisites

- Node.js 18+
- Emscripten (for Web builds)
- CMake 3.18+ (for Android)
- Xcode 14+ (for iOS)

### Build Commands

```bash
# Install dependencies
npm install

# Build Web (WASM)
npm run build:web

# Build TypeScript
npm run build:ts

# Build all
npm run build

# Clean
npm run clean
```

## License

MIT © Nathaniel Potrepka

## Credits

Based on the [DSP library](https://github.com/potrepka/DSP) by Nathaniel Potrepka.

Includes the following embedded libraries:

- [FFTConvolver](https://github.com/HiFi-LoFi/FFTConvolver) by HiFi-LoFi
- [simd_math_prims](https://github.com/to-miz/simd_math_prims) by Tolga Mizrak
