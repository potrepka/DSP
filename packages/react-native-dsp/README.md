# @potrepka/react-native-dsp

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
npm install @potrepka/react-native-dsp
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
import { Biquad, FilterType } from '@potrepka/react-native-dsp'

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
  Biquad,
  Buffer,
  CompressorGate,
  DryWet,
  Engine,
  MidiBuffer,
  Phasor,
  Space,
  TableOscillator,
  Type,
  VariableDelay,
} from '@potrepka/react-native-dsp'

const sampleRate = 48000
const numChannels = 2
const numSamples = 128

// Create engine
const engine = new Engine(-1, -1, numSamples, sampleRate)

// Create nodes
const phasor = new Phasor()
const osc = new TableOscillator()
const filter = new Biquad()
const compressor = new CompressorGate()
const delay = new VariableDelay()
const dryWet = new DryWet()

// Configure number of channels
phasor.setNumChannels(numChannels)
osc.setNumChannels(numChannels)
filter.setNumChannels(numChannels)
compressor.setNumChannels(numChannels)
delay.setNumChannels(numChannels)
dryWet.setNumChannels(numChannels)

// Add nodes
const root = engine.getNodeProcessor().getDefaultNode()
root.addChild(phasor)
root.addChild(osc)
root.addChild(filter)
root.addChild(compressor)
root.addChild(delay)
root.addChild(dryWet)

// Configure parameters
phasor.setFrequency(220)
osc.setWaveform('square')
filter.setFrequency(1760)
delay.setMaxDelayTime(0.25)
delay.setDelayTime(0.25)
compressor.setThreshold(0.5)
compressor.setRatio(4)
dryWet.setWet(0.5)

// Phase
phasor.getOutput.connect(osc.getPhase())

// Effects
osc.getOutput().connect(filter.getInput())
filter.getOutput().connect(delay.getInput())
delay.getOutput().connect(compressor.getInput())

// Dry/Wet
osc.getOutput().connect(dryWet.getDry())
compressor.getOutput().connect(dryWet.getWet())

// Output
dryWet.getOutput().connect(engine.getNodeProcessor().getAudioOutput())

// Process audio
const buffer = new Buffer(Type.RATIO, Space.TIME, 0, 0, numChannels, numSamples)
const audioBuffer = buffer.getData()
const midiBuffer = new MidiBuffer()
engine.getNodeProcessor().process(audioBuffer, midiBuffer)

// Print output
console.log(audioBuffer.getArrayOfReadPointers())

// Cleanup
engine.dispose()
buffer.dispose()
midiBuffer.dispose()
phasor.dispose()
osc.dispose()
filter.dispose()
compressor.dispose()
delay.dispose()
dryWet.dispose()
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
import { Platform } from '@potrepka/react-native-dsp'

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
