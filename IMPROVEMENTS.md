# DSP Library Improvements

## Current Architecture

### C++ Core (`src/`)

A node-based DSP graph compiled to WebAssembly:

- Base abstractions: `Node`, `Producer`, `Consumer`, `Transformer`
- `Input`/`Output` buffers with automatic connection management
- Processing organized by categories: Generators, Filters, Dynamics, Math, Triggers, etc.
- MIDI support via `MidiProcessor` and `MidiBuffer`

### TypeScript Layer (`packages/react-native-dsp/src/`)

Proxy layer for WASM:

- `DSP` class with ~70 factory methods (`createPhasor`, `createBiquad`, etc.)
- Async RPC-style communication via `MessagePort`
- Chainable promise API (`Chainable<T>`)
- Tied to `AudioWorkletProcessor`

## Key Limitations

| Area                   | Issue                                                                                                         |
| ---------------------- | ------------------------------------------------------------------------------------------------------------- |
| **Domain Scope**       | Audio/synthesis-focused. Nodes named for audio (Phasor, Envelope, Biquad) rather than generic DSP primitives. |
| **Platform Coupling**  | Hardcoded to Web Audio API. No pure-TS fallback, no Node.js support.                                          |
| **Signal Model**       | Only continuous sample streams. No discrete events, no multidimensional data (images, tensors).               |
| **API Ergonomics**     | Verbose factory pattern. No operator overloading or DSL.                                                      |
| **Missing Primitives** | No window functions, resampling, correlation, statistical analysis, general convolution.                      |
| **Type System**        | `Type` enum mixes semantic meaning (HERTZ, SECONDS) with representation (INTEGER, BOOLEAN).                   |

## Plan for General-Purpose DSP Library in TypeScript

### 1. Decouple from Audio Worklet

**Goal**: Make the core usable in any JavaScript runtime.

**Steps**:

- Create a `PureTypeScriptProcessor` that runs the WASM module synchronously
- Add a `Runtime` abstraction:
  ```typescript
  type Runtime = 'web-audio' | 'web-worker' | 'node' | 'synchronous'
  ```
- Move `AudioWorkletProcessor` to a separate `@dsp/web-audio` package
- Allow direct WASM instantiation for CLI tools, tests, and offline processing

### 2. Rename and Reorganize Nodes by DSP Function

**Goal**: Make node names reflect general DSP concepts.

| Current           | Proposed General Name                 |
| ----------------- | ------------------------------------- |
| `Phasor`          | `Ramp` or `PhaseAccumulator`          |
| `Envelope`        | `ASRCurve` (Attack-Sustain-Release)   |
| `Biquad`          | `IIRFilter` (expose as biquad preset) |
| `TableOscillator` | `WavetableLookup`                     |
| `SamplePlayer`    | `BufferReader`                        |
| `Lag`             | `Slew` or `OnePoleFilter`             |
| `ClockTrigger`    | `PeriodicTrigger`                     |

**New Category Structure**:

```
Sources/              (produce signals without input)
  - Ramp
  - Noise
  - Constant

LinearFilters/        (IIR, FIR)
  - IIRFilter (modes: lowpass, highpass, bandpass, etc.)
  - FIRFilter
  - Convolver

NonlinearProcessors/
  - Envelope (ASR, ADSR)
  - Compressor
  - Waveshaper

Operations/           (arithmetic on signals)
  - Add, Multiply, Divide
  - Power, Log, Exp
  - Trigonometric, Hyperbolic

FrequencyDomain/
  - FFT, IFFT
  - WindowFunction
  - SpectralAnalyzer

ControlFlow/          (triggers, gates)
  - Trigger, Gate
  - SampleAndHold
  - Sequencer

External/
  - MidiInput, MidiOutput
  - OSCInput, OSCOutput
```

### 3. Add Missing DSP Primitives

**Window Functions**:

```typescript
type WindowType = 'rectangular' | 'hanning' | 'hamming' | 'blackman' | 'kaiser'
createWindow(size: number, type: WindowType): Float64Array
```

**Resampling**:

```typescript
createResampler({
  inputRate: number,
  outputRate: number,
  quality: 'low' | 'medium' | 'high',
})
```

**Statistical Analysis**:

```typescript
createAnalyzer({
  metrics: ['rms', 'peak', 'crest', 'zeroCrossings', 'centroid'],
})
```

**General Convolution**:

```typescript
createConvolution({
  mode: 'linear' | 'circular',
  normalize: boolean,
})
```

### 4. Simplify the TypeScript API

**Current** (verbose):

```typescript
const dsp = new DSP(workletNode)
const phasor = await dsp.createPhasor()
const freq = await phasor.getFrequency()
await freq.setChannelValue(0, 440)
const output = await phasor.getOutput()
const audioOut = dsp.getNodeProcessor().getAudioOutput()
await output.connect(audioOut)
```

**Proposed** (fluent builder):

```typescript
const graph = dsp.graph()
const osc = graph.ramp({ frequency: 440 })
const filtered = graph
  .iirFilter({ mode: 'lowpass', frequency: 1000 })
  .input(osc)
graph.output(filtered)
await graph.build()
```

**Add operator-style connections**:

```typescript
;((osc.output >> filter.input) >> gain.input) >> graph.output
```

### 5. Support Multiple Signal Domains

**Current**: Only audio samples (time-domain float streams).

**Proposed**: Generalize `Buffer` to handle:

| Domain      | Description                             |
| ----------- | --------------------------------------- |
| `time`      | Continuous samples (current)            |
| `frequency` | Complex spectra (magnitude/phase)       |
| `event`     | Discrete messages (MIDI, OSC, JSON)     |
| `tensor`    | N-dimensional arrays (for ML/image DSP) |

Add domain converters:

```typescript
const spectrum = graph.fft(signal)
const filtered = graph.spectralFilter(spectrum, {
  mode: 'bandpass',
  low: 100,
  high: 5000,
})
const output = graph.ifft(filtered)
```

### 6. Pure TypeScript Fallback

Create `@dsp/core-ts` with pure TypeScript implementations of all nodes for:

- Testing without WASM
- Environments where WASM is not available
- Educational/debugging purposes

The API remains identical; only the runtime changes.

### 7. Package Structure

```
@dsp/core          - Core types, interfaces, graph builder
@dsp/wasm          - WebAssembly implementations
@dsp/ts            - Pure TypeScript implementations
@dsp/web-audio     - Web Audio API integration
@dsp/node          - Node.js bindings (native audio I/O)
@dsp/react-native  - React Native integration (existing)
```

### 8. Documentation and Examples

- Interactive playground (like Tone.js examples)
- Category-based documentation:
  - Filtering Signals
  - Spectral Analysis
  - Building Synthesizers
  - Real-time Audio Effects
- TypeScript-first API reference with full type information

## Summary

The library has a solid C++/WASM foundation but is currently specialized for audio synthesis in Web Audio contexts. To make it general-purpose DSP accessible in TypeScript:

1. **Decouple** from Web Audio API (runtime abstraction)
2. **Rename** nodes to reflect DSP concepts, not audio-specific terms
3. **Add** missing primitives (windows, resampling, analysis, general convolution)
4. **Simplify** the API with a fluent graph builder
5. **Generalize** signal domains beyond audio samples
6. **Provide** pure TypeScript fallbacks
7. **Modularize** into focused packages
