# @potrepka/react-native-dsp

High-performance audio DSP library for TypeScript with Web and React Native support.

## Table of Contents

- [Installation](#installation)
- [Quick Start](#quick-start)
  - [Web Setup](#web-setup)
  - [React Native Setup](#react-native-setup)
- [Core Concepts](#core-concepts)
  - [DSP Class](#dsp-class)
  - [Node Processor](#node-processor)
  - [Buffers, Wrappers, and Data](#buffers-wrappers-and-data)
  - [Nodes](#nodes)
  - [Node Creation Defaults](#node-creation-defaults)
  - [Chainable API](#chainable-api)
  - [Connecting Nodes](#connecting-nodes)
  - [Locking](#locking)
  - [Vectors](#vectors)
  - [Deleting Objects](#deleting-objects)
  - [TypeScript Support](#typescript-support)
- [Node Types](#node-types)
  - [Analyzer Nodes](#analyzer-nodes)
  - [Channel Nodes](#channel-nodes)
  - [Delay Nodes](#delay-nodes)
  - [Dynamics Nodes](#dynamics-nodes)
  - [External Nodes](#external-nodes)
  - [Filter Nodes](#filter-nodes)
  - [Generator Nodes](#generator-nodes)
  - [Math Nodes](#math-nodes)
  - [Trigger Nodes](#trigger-nodes)
  - [Variable Nodes](#variable-nodes)
- [MIDI Support](#midi-support)
  - [MIDI Buffers](#midi-buffers)
  - [MIDI Messages](#midi-messages)
- [FFT Support](#fft-support)
  - [FFT Nodes](#fft-nodes)
  - [NormalizedFFT](#normalizedfft)
- [Enums](#enums)
  - [Global Enums](#global-enums)
  - [Node Enums](#node-enums)
- [Utility Functions](#utility-functions)
- [Constants](#constants)
- [Platform Detection](#platform-detection)
- [License](#license)

## Installation

```bash
bun install @potrepka/react-native-dsp
```

## Quick Start

### Web Setup

```typescript
import { DSP, initializeWebAudio } from '@potrepka/react-native-dsp'
import wasmModuleUrl from '@potrepka/react-native-dsp/web/build/react-native-dsp.js?url'

const audioContext = new AudioContext({ sampleRate: 48000 })

const { addModule, createAudioWorkletNode } =
  await initializeWebAudio(wasmModuleUrl)

await addModule(audioContext)

const workletNode = createAudioWorkletNode(audioContext, {
  numInputChannels: 0,
  numOutputChannels: audioContext.destination.channelCount,
  numSamples: 128,
  sampleRate: audioContext.sampleRate,
})

const dsp = new DSP(workletNode)
await dsp.isReady()

const phasor = await dsp.createPhasor()
const osc = await dsp.createTableOscillator()
const filter = await dsp.createBiquad()
const gain = await dsp.createMultiplication()

await phasor.setNumChannels(2)
await osc.setNumChannels(2)
await filter.setNumChannels(2)
await gain.setNumChannels(2)

await phasor.getFrequency().setAllChannelValues(55)
await filter.getFrequency().setAllChannelValues(880)
await gain.getFactor().setAllChannelValues(0.5)

const sawtoothBufferSize = 2048
const sawtooth = await dsp.createBuffer({
  numChannels: 1,
  numSamples: sawtoothBufferSize,
})
const sawtoothWrapper = await sawtooth.getWrapper()
for (let sample = 0; sample < sawtoothBufferSize; sample++) {
  const phase = sample / sawtoothBufferSize
  const value = 2 * ((phase + 0.5) % 1) - 1
  sawtoothWrapper.setSample(0, sample, value)
}

await osc.getTables().push_back(sawtooth)

await phasor.getOutput().connect(await osc.getPhase())
await osc.getOutput().connect(await filter.getInput())
await filter.getOutput().connect(await gain.getInput())
await gain.getOutput().connect(await dsp.getNodeProcessor().getAudioOutput())

workletNode.connect(audioContext.destination)

await new Promise((resolve) => setTimeout(resolve, 1000))

workletNode.disconnect()
await audioContext.close()
await dsp.delete()
```

### React Native Setup

React Native support requires `react-native-audio-api` for audio processing via JSI.

```typescript
import { DSP } from '@potrepka/react-native-dsp'
```

## Core Concepts

### DSP Class

The `DSP` class is the main entry point for creating and managing DSP objects. It communicates with the audio worklet processor via message passing.

```typescript
const dsp = new DSP(workletNode)
await dsp.isReady()
```

### Node Processor

The `NodeProcessor` manages the audio processing graph. Access it via `dsp.getNodeProcessor()`. When using `initializeWebAudio`, the NodeProcessor is automatically configured with the channel counts, sample count, and sample rate specified in the options.

```typescript
const nodeProcessor = dsp.getNodeProcessor()

const audioInput = await nodeProcessor.getAudioInput()
const audioOutput = await nodeProcessor.getAudioOutput()
const audioInputClipping = await nodeProcessor.getAudioInputClipping()
const audioOutputClipping = await nodeProcessor.getAudioOutputClipping()

const numInputChannels = await nodeProcessor.getNumInputChannels()
const numOutputChannels = await nodeProcessor.getNumOutputChannels()
const numSamples = await nodeProcessor.getNumSamples()
const sampleRate = await nodeProcessor.getSampleRate()

await nodeProcessor.setNumInputChannels(2)
await nodeProcessor.setNumOutputChannels(2)
await nodeProcessor.setNumSamples(128)
await nodeProcessor.setSampleRate(48000)
await nodeProcessor.setInputSize(2, 128)
await nodeProcessor.setOutputSize(2, 128)

await nodeProcessor.isActive()
await nodeProcessor.setActive(true)

const defaultNode = await nodeProcessor.getDefaultNode()
const nodes = await nodeProcessor.getNodes()

const inputMessages = await nodeProcessor.getInputMessages()
const outputMessages = await nodeProcessor.getOutputMessages()
```

### Buffers, Wrappers, and Data

The library provides three related classes for working with audio data:

**Buffer** is the high-level container used for inputs, outputs, and tables. Buffers contain audio data along with metadata (type, space, range, and default value). Input and Output both extend Buffer.

Each Buffer channel has a channel value, which is set to the default value when a channel is created. During processing, if an Input has connections, the connected data is used; otherwise, the Input's channel values are used. Consequently, the channel values can be used to set static, persistent input values for nodes.

If range is greater than zero, the Input or Output buffers are clipped to `[0, range]` during processing.

For `Type.BOOLEAN` buffers, values are quantized to `0.0` (for zero values) or `1.0` (for non-zero values) during processing.

For `Type.INTEGER` buffers, values are quantized to the greatest integer less than or equal to the value during processing.

```typescript
const buffer = await dsp.createBuffer({
  numChannels: 2,
  numSamples: 128,
})
await buffer.setType(Type.RATIO)
await buffer.setSpace(Space.TIME)
await buffer.setRange(0.0)
await buffer.setDefaultValue(0.0)
await buffer.setNumChannels(2)
await buffer.setNumSamples(128)
await buffer.setSize(2, 128)

await buffer.setChannelValue(0, 0.5)
await buffer.setChannelValues(new Float64Array([0.5, 0.5]))
await buffer.setAllChannelValues(0.5)

const peak = await buffer.getPeak()
const rms = await buffer.getRMS()
```

**Wrapper** provides a view into audio sample data with methods for manipulation. You can get a Wrapper from a Buffer.

```typescript
const wrapper = await buffer.getWrapper()

const numChannels = await wrapper.getNumChannels()
const numSamples = await wrapper.getNumSamples()

const sample = await wrapper.getSample(0, 0)
await wrapper.setSample(0, 0, 0.5)

await wrapper.clear()
await wrapper.fill(0.5)
await wrapper.copyFrom(otherWrapper)

await wrapper.add(0.5)
await wrapper.addWrapper(otherWrapper)
await wrapper.multiplyBy(0.5)
await wrapper.multiplyByWrapper(otherWrapper)

await wrapper.addProductOf(otherWrapper, 0.5)
await wrapper.addProductOfWrappers(wrapper1, wrapper2)

await wrapper.replaceWithNegativeOf(otherWrapper)
await wrapper.replaceWithAbsoluteValueOf(otherWrapper)
await wrapper.replaceWithSumOf(otherWrapper, 0.5)
await wrapper.replaceWithSumOfWrappers(wrapper1, wrapper2)
await wrapper.replaceWithProductOf(otherWrapper, 0.5)
await wrapper.replaceWithProductOfWrappers(wrapper1, wrapper2)
await wrapper.replaceWithMinOf(otherWrapper, 0.5)
await wrapper.replaceWithMinOfWrappers(wrapper1, wrapper2)
await wrapper.replaceWithMaxOf(otherWrapper, 0.5)
await wrapper.replaceWithMaxOfWrappers(wrapper1, wrapper2)

const singleChannelWrapper = await wrapper.getSingleChannel(0)
const sampleRangeWrapper = await wrapper.getSampleRange(0, 512)
```

**Data** is the underlying raw sample storage. You can get Data from a Buffer. The `getReadChannelData()` method returns a single channel as an array, while `getReadData()` returns all channels as a 2D array (channels × samples).

```typescript
const data = await buffer.getData()

const numChannels = await data.getNumChannels()
const numSamples = await data.getNumSamples()
await data.setSize(2, 128)
await data.clear()

const channelData = await data.getReadChannelData(0)
const allData = await data.getReadData()

const peakLevel = await data.getPeakLevel(0, 0, 128)
const rmsLevel = await data.getRMSLevel(0, 0, 128)
```

### Nodes

Nodes are the building blocks of the audio processing graph. There are four base node types:

**Node** is the base class for all processing nodes. It provides common functionality for managing channels, samples, sample rate, inputs, outputs, and child nodes.

Nodes can be added as children to other nodes to create hierarchical processing graphs. The most common use case is to add nodes to the default node of the NodeProcessor. When a node is added as a child to another node, the `numSamples` and `sampleRate` of the parent node are automatically propagated to the child node.

```typescript
await node.setNumChannels(2)
await node.setNumInputChannels(2)
await node.setNumOutputChannels(2)
await node.setNumSamples(128)
await node.setSampleRate(48000)

await node.isActive()
await node.setActive(true)

const inputs = await node.getInputs()
const outputs = await node.getOutputs()
const children = await node.getChildren()

await node.addChild(childNode)
await node.removeChild(childNode)
await node.sortChildren()
await node.disconnectAll()
await node.process()
```

**Consumer** extends Node and has a single input. Used for nodes that only receive signals.

```typescript
const input = await consumer.getInput()
```

**Producer** extends Node and has a single output. Used for nodes that only generate signals.

```typescript
const output = await producer.getOutput()
```

**Transformer** extends Node and has both an input and an output. Used for nodes that process signals.

```typescript
const input = await transformer.getInput()
const output = await transformer.getOutput()
```

### Node Creation Defaults

When creating nodes, most parameters are optional. Default values are:

- `type`: `Type.RATIO`
- `space`: `Space.TIME`

Some nodes require specific parameters:

- `MidiInput` requires a `midiBuffer`.
- `MidiOutput` requires a `midiBuffer`.

Check individual node documentation for required options.

### Chainable API

All proxy methods return `Chainable<T>` objects that support both `await` and method chaining.

```typescript
const phasor = await dsp.createPhasor()
const freq = await phasor.getFrequency()
await freq.setAllChannelValues(440)

await phasor.getFrequency().setAllChannelValues(440)
```

### Connecting Nodes

Nodes are connected by linking outputs to inputs. An output can connect to multiple inputs, and an input can receive connections from multiple outputs.

```typescript
const phasor = await dsp.createPhasor()
const osc = await dsp.createTableOscillator()
const filter = await dsp.createBiquad()

await phasor.getOutput().connect(await osc.getPhase())
await osc.getOutput().connect(await filter.getInput())

const connections = await filter.getInput().getConnections()

await osc.getOutput().disconnect(await filter.getInput())
await phasor.getOutput().disconnectAll()
```

Inputs support different modes for combining multiple connections:

```typescript
import { InputMode } from '@potrepka/react-native-dsp'

const input = await node.getInput()
const mode = await input.getMode()

await input.setMode(InputMode.SUM) // Sum all connected inputs (default)
await input.setMode(InputMode.MINIMUM) // Use minimum of all connected inputs
await input.setMode(InputMode.MAXIMUM) // Use maximum of all connected inputs
```

### Locking

The library provides a `LockableProxy` class for thread-safe operations. The same class is used throughout the library to provide thread-safe operations.

```typescript
import { LockableProxy } from '@potrepka/react-native-dsp'

await lockable.lock()
await lockable.unlock()
```

### Vectors

C++ vectors are exposed to TypeScript for certain operations, such as managing tables or sequences. The `VectorProxy` class provides access to vector operations.

```typescript
const tableOscillator = await dsp.createTableOscillator()
const tables = await tableOscillator.getTables()

const table = await tables.get(0)
await tables.push_back(buffer)
await tables.set(0, buffer)
const count = await tables.size()
await tables.resize(4, defaultBuffer)
```

### Deleting Objects

When you are finished using the DSP instance, you must call the `delete()` method to clean up all resources. This is critically important when using WebAssembly to avoid memory leaks.

```typescript
await dsp.delete()
```

### TypeScript Support

The library provides full TypeScript support with exported types for all classes and enums. All proxy methods return `Chainable<T>` for type-safe chaining, and node creation options are fully typed.

```typescript
import type {
  Buffer,
  Input,
  Output,
  Node,
  Consumer,
  Producer,
  Transformer,
  Chainable,
} from '@potrepka/react-native-dsp'
```

## Node Types

### Analyzer Nodes

#### Recorder

Records audio input to a buffer. Extends Consumer.

```typescript
const recorder = await dsp.createRecorder({
  type: Type.RATIO,
  space: Space.TIME,
  defaultValue: 0,
})
await recorder.setNumChannels(2)

// Inputs
const input = await recorder.getInput()
const gate = await recorder.getGate()
const reset = await recorder.getReset()

// Miscellaneous
await recorder.getMode()
await recorder.setMode(RecorderMode.FIXED_SINGLE)
await recorder.getRecordingTime()
await recorder.setRecordingTime(1.0)
const recordingBuffer = await recorder.getRecordingBuffer()
```

### Channel Nodes

#### ChannelMerger

Merges multiple mono inputs into a multi-channel output. Extends Producer.

```typescript
const channelMerger = await dsp.createChannelMerger({
  type: Type.RATIO,
  space: Space.TIME,
})
await channelMerger.setNumChannels(2)

// Inputs
const input0 = await channelMerger.getInput(0)
const input1 = await channelMerger.getInput(1)

// Outputs
const output = await channelMerger.getOutput()
```

#### ChannelSplitter

Splits a multi-channel input into separate mono outputs. Extends Consumer.

```typescript
const channelSplitter = await dsp.createChannelSplitter({
  type: Type.RATIO,
  space: Space.TIME,
})
await channelSplitter.setNumChannels(2)

// Inputs
const input = await channelSplitter.getInput()

// Outputs
const output0 = await channelSplitter.getOutput(0)
const output1 = await channelSplitter.getOutput(1)
```

#### MidSide

Converts stereo to mid/side encoding. Extends Consumer.

```typescript
const midSide = await dsp.createMidSide({
  type: Type.RATIO,
  space: Space.TIME,
})
await midSide.setNumChannels(2)

// Inputs
const input = await midSide.getInput()
const mixAmount = await midSide.getMixAmount()

// Outputs
const mid = await midSide.getMid()
const side = await midSide.getSide()
```

#### Spread

Applies stereo spread to a signal. Extends Transformer.

```typescript
const spread = await dsp.createSpread({
  type: Type.RATIO,
  space: Space.TIME,
})
await spread.setNumChannels(2)

// Inputs
const input = await spread.getInput()
const spreadAmount = await spread.getSpread()

// Outputs
const output = await spread.getOutput()

// Miscellaneous
await spread.getMode()
await spread.setMode(SpreadMode.BIPOLAR)
```

#### StereoPanner

Pans a mono signal to stereo. Extends Consumer.

```typescript
const stereoPanner = await dsp.createStereoPanner({
  type: Type.RATIO,
  space: Space.TIME,
})
await stereoPanner.setNumChannels(2)

// Inputs
const input = await stereoPanner.getInput()
const direction = await stereoPanner.getDirection()

// Outputs
const left = await stereoPanner.getLeft()
const right = await stereoPanner.getRight()
```

### Delay Nodes

#### Convolver

Applies convolution reverb using an impulse response. Extends Transformer.

```typescript
const convolver = await dsp.createConvolver()
await convolver.setNumChannels(2)

// Inputs
const input = await convolver.getInput()

// Outputs
const output = await convolver.getOutput()

// Miscellaneous
await convolver.getHeadSize()
await convolver.setHeadSize(512)
await convolver.getTailSize()
await convolver.setTailSize(4096)
await convolver.getBuffer()
await convolver.setBuffer(irBuffer)
await convolver.initConvolvers()
```

#### VariableDelay

Variable-length delay with feedback. Extends Transformer.

```typescript
const variableDelay = await dsp.createVariableDelay({ type: Type.RATIO })
await variableDelay.setNumChannels(2)

// Inputs
const input = await variableDelay.getInput()
const delayTime = await variableDelay.getDelayTime()
const decayTime = await variableDelay.getDecayTime()
const reset = await variableDelay.getReset()
const feedbackSink = await variableDelay.getFeedbackSink()

// Outputs
const output = await variableDelay.getOutput()
const feedbackSource = await variableDelay.getFeedbackSource()

// Miscellaneous
await variableDelay.getMaxDelayTime()
await variableDelay.setMaxDelayTime(2.0)
const feedbackProcessor = await variableDelay.getFeedbackProcessor()
```

### Dynamics Nodes

#### Clipper

Clips, wraps, or folds signal amplitude. Extends Transformer.

```typescript
const clipper = await dsp.createClipper({
  type: Type.RATIO,
  space: Space.TIME,
})
await clipper.setNumChannels(2)

// Inputs
const input = await clipper.getInput()
const minimum = await clipper.getMinimum()
const maximum = await clipper.getMaximum()

// Outputs
const output = await clipper.getOutput()

// Miscellaneous
await clipper.getMode()
await clipper.setMode(ClipperMode.CLIP)
```

#### CompressorGate

Dynamics processor with compression and gating. Extends Transformer.

```typescript
const compressorGate = await dsp.createCompressorGate()
await compressorGate.setNumChannels(2)

// Inputs
const input = await compressorGate.getInput()
const control = await compressorGate.getControl()
const threshold = await compressorGate.getThreshold()
const softness = await compressorGate.getSoftness()
const compressionRatio = await compressorGate.getCompressionRatio()
const gateRatio = await compressorGate.getGateRatio()
const attack = await compressorGate.getAttack()
const release = await compressorGate.getRelease()
const gain = await compressorGate.getGain()

// Outputs
const output = await compressorGate.getOutput()

// Analysis: get gain reduction for a given input level (useful for drawing gain curves)
const gainResponse = await compressorGate.getGainResponse(0, 0.5)
```

#### DryWet

Mixes between dry and wet signals (or A and B). Extends Producer.

```typescript
const dryWet = await dsp.createDryWet({
  type: Type.RATIO,
  space: Space.TIME,
})
await dryWet.setNumChannels(2)

// Inputs
const dry = await dryWet.getDry()
const wet = await dryWet.getWet()
const mixAmount = await dryWet.getMixAmount()
const a = await dryWet.getA()
const b = await dryWet.getB()

// Outputs
const output = await dryWet.getOutput()
```

#### Envelope

ADSR-style envelope generator. Extends Producer.

```typescript
const envelope = await dsp.createEnvelope()
await envelope.setNumChannels(2)

// Inputs
const attack = await envelope.getAttack()
const release = await envelope.getRelease()
const gate = await envelope.getGate()
const reset = await envelope.getReset()

// Outputs
const output = await envelope.getOutput()

// Miscellaneous
await envelope.getAttackShape()
await envelope.setAttackShape(Shape.EXPONENTIAL)
await envelope.getReleaseShape()
await envelope.setReleaseShape(Shape.LINEAR)
const currentTime = await envelope.getCurrentTime()
```

#### Lag

Smooths parameter changes over time. Extends Transformer.

```typescript
const lag = await dsp.createLag({ type: Type.RATIO })
await lag.setNumChannels(2)

// Inputs
const input = await lag.getInput()
const lagTime = await lag.getLagTime()

// Outputs
const output = await lag.getOutput()
```

#### Shaper

Waveshaping distortion. Extends Transformer.

```typescript
const shaper = await dsp.createShaper({ space: Space.TIME })
await shaper.setNumChannels(2)

// Inputs
const input = await shaper.getInput()
const drive = await shaper.getDrive()

// Outputs
const output = await shaper.getOutput()

// Miscellaneous
await shaper.getMode()
await shaper.setMode(ShaperMode.HYPERBOLIC)

// Analysis: get shaped output for a given input value (useful for drawing waveshaper curves)
const outputSample = await shaper.getOutputSample(0, 0.5)
```

#### TableShaper

Table-based waveshaping with interpolation. Extends Transformer.

```typescript
const tableShaper = await dsp.createTableShaper({
  outputType: Type.RATIO,
  space: Space.TIME,
})
await tableShaper.setNumChannels(2)

// Inputs
const input = await tableShaper.getInput()
const position = await tableShaper.getPosition()

// Outputs
const output = await tableShaper.getOutput()

// Miscellaneous
const tables = await tableShaper.getTables()
await tableShaper.getInputInterpolation()
await tableShaper.setInputInterpolation(Interpolation.HERMITE)
await tableShaper.getPositionInterpolation()
await tableShaper.setPositionInterpolation(Interpolation.LINEAR)
```

### External Nodes

#### MidiInput

Receives MIDI messages and converts to control signals. Extends Producer.

```typescript
const midiBuffer = await dsp.createMidiBuffer()
const midiInput = await dsp.createMidiInput({
  midiBuffer,
  type: Type.RATIO,
})
await midiInput.setNumChannels(2)

// Outputs
const output = await midiInput.getOutput()

// Miscellaneous
const inputMessages = await midiInput.getInputMessages()
```

#### MidiOutput

Converts control signals to MIDI messages. Extends Consumer.

```typescript
const midiBuffer = await dsp.createMidiBuffer()
const midiOutput = await dsp.createMidiOutput({
  midiBuffer,
  type: Type.RATIO,
})
await midiOutput.setNumChannels(2)

// Inputs
const input = await midiOutput.getInput()

// Miscellaneous
const outputMessages = await midiOutput.getOutputMessages()
```

### Filter Nodes

#### Biquad

Second-order IIR filter with multiple modes. Extends Transformer.

```typescript
const biquad = await dsp.createBiquad()
await biquad.setNumChannels(2)

// Inputs
const input = await biquad.getInput()
const frequency = await biquad.getFrequency()
const resonance = await biquad.getResonance()
const amplitude = await biquad.getAmplitude()

// Outputs
const output = await biquad.getOutput()

// Miscellaneous
await biquad.getMode()
await biquad.setMode(BiquadMode.LOW_PASS)

// Analysis: get magnitude response at a specific frequency (useful for visualization)
const frequencyResponse = await biquad.getFrequencyResponse(1000)
```

#### Crossover

Splits signal into low and high frequency bands. Extends Node directly (not Consumer/Producer/Transformer) because it uses child nodes for processing.

```typescript
const crossover = await dsp.createCrossover()
await crossover.setNumChannels(2)

// Inputs
const input = await crossover.getInput()
const frequency = await crossover.getFrequency()

// Outputs
const low = await crossover.getLow()
const high = await crossover.getHigh()
```

#### OnePole

Simple one-pole lowpass or highpass filter. Extends Transformer.

```typescript
const onePole = await dsp.createOnePole({ type: Type.RATIO })
await onePole.setNumChannels(2)

// Inputs
const input = await onePole.getInput()
const frequency = await onePole.getFrequency()

// Outputs
const output = await onePole.getOutput()

// Miscellaneous
await onePole.getMode()
await onePole.setMode(OnePoleMode.LOW_PASS)
```

### Generator Nodes

#### Noise

White or pink noise generator. Extends Producer.

```typescript
const noise = await dsp.createNoise()
await noise.setNumChannels(2)

// Outputs
const output = await noise.getOutput()

// Miscellaneous
await noise.getMode()
await noise.setMode(NoiseMode.WHITE)
```

#### Phasor

Phase accumulator oscillator. Extends Producer.

```typescript
const phasor = await dsp.createPhasor()
await phasor.setNumChannels(2)

// Inputs
const frequency = await phasor.getFrequency()
const reset = await phasor.getReset()

// Outputs
const output = await phasor.getOutput()

// Miscellaneous
await phasor.getMode()
await phasor.setMode(PhasorMode.WRAPPED)
```

#### SamplePlayer

Plays back audio samples with variable speed. Extends Producer.

```typescript
const samplePlayer = await dsp.createSamplePlayer({ type: Type.RATIO })
await samplePlayer.setNumChannels(2)

// Inputs
const speed = await samplePlayer.getSpeed()
const startTime = await samplePlayer.getStartTime()
const sampleIndex = await samplePlayer.getSampleIndex()
const gate = await samplePlayer.getGate()
const reset = await samplePlayer.getReset()

// Outputs
const output = await samplePlayer.getOutput()

// Miscellaneous
const samples = await samplePlayer.getSamples()
await samplePlayer.getInterpolation()
await samplePlayer.setInterpolation(Interpolation.HERMITE)
const currentTime = await samplePlayer.getCurrentTime()
```

#### TableOscillator

Wavetable oscillator with position morphing. Extends Producer.

```typescript
const tableOscillator = await dsp.createTableOscillator({ type: Type.RATIO })
await tableOscillator.setNumChannels(2)

// Inputs
const phase = await tableOscillator.getPhase()
const position = await tableOscillator.getPosition()

// Outputs
const output = await tableOscillator.getOutput()

// Miscellaneous
const tables = await tableOscillator.getTables()
await tableOscillator.getPhaseInterpolation()
await tableOscillator.setPhaseInterpolation(Interpolation.HERMITE)
await tableOscillator.getPositionInterpolation()
await tableOscillator.setPositionInterpolation(Interpolation.LINEAR)
```

### Math Nodes

#### AbsoluteValue

Outputs the absolute value of the input. Extends Transformer.

```typescript
const absoluteValue = await dsp.createAbsoluteValue({
  type: Type.RATIO,
  space: Space.TIME,
})
await absoluteValue.setNumChannels(2)

// Inputs
const input = await absoluteValue.getInput()

// Outputs
const output = await absoluteValue.getOutput()
```

#### BooleanMask

Applies a boolean mask to the input signal. Extends Transformer.

```typescript
const booleanMask = await dsp.createBooleanMask({
  type: Type.RATIO,
  space: Space.TIME,
})
await booleanMask.setNumChannels(2)

// Inputs
const input = await booleanMask.getInput()
const mask = await booleanMask.getMask()

// Outputs
const output = await booleanMask.getOutput()
```

#### Comparison

Compares input against a threshold. Extends Transformer.

```typescript
const comparison = await dsp.createComparison({
  type: Type.RATIO,
  space: Space.TIME,
})
await comparison.setNumChannels(2)

// Inputs
const input = await comparison.getInput()
const threshold = await comparison.getThreshold()

// Outputs
const output = await comparison.getOutput()

// Miscellaneous
await comparison.getMode()
await comparison.setMode(ComparisonMode.GREATER_THAN)
```

#### Division

Divides input by a divisor. Extends Transformer.

```typescript
const division = await dsp.createDivision({
  type: Type.RATIO,
  space: Space.TIME,
})
await division.setNumChannels(2)

// Inputs
const input = await division.getInput()
const divisor = await division.getDivisor()

// Outputs
const output = await division.getOutput()
```

#### Floor

Quantizes input to integer multiples of divisor. Extends Transformer.

```typescript
const floor = await dsp.createFloor({
  type: Type.RATIO,
  space: Space.TIME,
})
await floor.setNumChannels(2)

// Inputs
const input = await floor.getInput()
const divisor = await floor.getDivisor()

// Outputs
const output = await floor.getOutput()
```

#### ForwardFFT

Converts time-domain signal to frequency-domain. Extends Consumer.

```typescript
const forwardFFT = await dsp.createForwardFFT()
await forwardFFT.setNumChannels(2)

// Inputs
const input = await forwardFFT.getInput()

// Outputs
const magnitude = await forwardFFT.getMagnitude()
const phase = await forwardFFT.getPhase()
```

#### FrequencyToNote

Converts frequency (Hz) to MIDI note number. Extends Transformer.

```typescript
const frequencyToNote = await dsp.createFrequencyToNote({ space: Space.TIME })
await frequencyToNote.setNumChannels(2)

// Inputs
const input = await frequencyToNote.getInput()
const tuningFrequency = await frequencyToNote.getTuningFrequency()

// Outputs
const output = await frequencyToNote.getOutput()
```

#### Function

Custom two-input function node. Extends Transformer.

```typescript
const func = await dsp.createFunction({
  type: Type.RATIO,
  space: Space.TIME,
})
await func.setNumChannels(2)

// Inputs
const input = await func.getInput()
const a = await func.getA()
const b = await func.getB()

// Outputs
const output = await func.getOutput()

// Miscellaneous
await func.getFunction()
await func.setFunction((a, b) => a + b)
```

#### Hyperbolic

Hyperbolic trigonometric functions (sinh, cosh, tanh). Extends Transformer.

```typescript
const hyperbolic = await dsp.createHyperbolic({ space: Space.TIME })
await hyperbolic.setNumChannels(2)

// Inputs
const input = await hyperbolic.getInput()

// Outputs
const output = await hyperbolic.getOutput()

// Miscellaneous
await hyperbolic.getMode()
await hyperbolic.setMode(HyperbolicMode.TANGENT)
```

#### Identity

Passes input through unchanged (useful for type/space conversion). Extends Transformer.

```typescript
const identity = await dsp.createIdentity({
  type: Type.RATIO,
  space: Space.TIME,
})
await identity.setNumChannels(2)

// Inputs
const input = await identity.getInput()

// Outputs
const output = await identity.getOutput()
```

#### InverseFFT

Converts frequency-domain signal back to time-domain. Extends Producer.

```typescript
const inverseFFT = await dsp.createInverseFFT()
await inverseFFT.setNumChannels(2)

// Inputs
const magnitude = await inverseFFT.getMagnitude()
const phase = await inverseFFT.getPhase()

// Outputs
const output = await inverseFFT.getOutput()
```

#### Logarithm

Computes logarithm of input. Extends Transformer.

```typescript
const logarithm = await dsp.createLogarithm({ space: Space.TIME })
await logarithm.setNumChannels(2)

// Inputs
const input = await logarithm.getInput()
const base = await logarithm.getBase()

// Outputs
const output = await logarithm.getOutput()
```

#### Modulo

Computes input modulo divisor. Extends Transformer.

```typescript
const modulo = await dsp.createModulo({
  type: Type.RATIO,
  space: Space.TIME,
})
await modulo.setNumChannels(2)

// Inputs
const input = await modulo.getInput()
const divisor = await modulo.getDivisor()

// Outputs
const output = await modulo.getOutput()
```

#### Multiplication

Multiplies input by a factor. Extends Transformer.

```typescript
const multiplication = await dsp.createMultiplication({
  type: Type.RATIO,
  space: Space.TIME,
})
await multiplication.setNumChannels(2)

// Inputs
const input = await multiplication.getInput()
const factor = await multiplication.getFactor()

// Outputs
const output = await multiplication.getOutput()
```

#### Negative

Outputs the negative of the input. Extends Transformer.

```typescript
const negative = await dsp.createNegative({
  type: Type.RATIO,
  space: Space.TIME,
})
await negative.setNumChannels(2)

// Inputs
const input = await negative.getInput()

// Outputs
const output = await negative.getOutput()
```

#### NoteToFrequency

Converts MIDI note number to frequency (Hz). Extends Transformer.

```typescript
const noteToFrequency = await dsp.createNoteToFrequency({ space: Space.TIME })
await noteToFrequency.setNumChannels(2)

// Inputs
const input = await noteToFrequency.getInput()
const tuningFrequency = await noteToFrequency.getTuningFrequency()

// Outputs
const output = await noteToFrequency.getOutput()
```

#### NotGate

Boolean NOT operation. Extends Transformer.

```typescript
const notGate = await dsp.createNotGate({ space: Space.TIME })
await notGate.setNumChannels(2)

// Inputs
const input = await notGate.getInput()

// Outputs
const output = await notGate.getOutput()
```

#### Power

Raises input to an exponent. Extends Transformer.

```typescript
const power = await dsp.createPower({ space: Space.TIME })
await power.setNumChannels(2)

// Inputs
const input = await power.getInput()
const exponent = await power.getExponent()

// Outputs
const output = await power.getOutput()
```

#### Reciprocal

Outputs 1/input. Extends Transformer.

```typescript
const reciprocal = await dsp.createReciprocal({
  type: Type.RATIO,
  space: Space.TIME,
})
await reciprocal.setNumChannels(2)

// Inputs
const input = await reciprocal.getInput()

// Outputs
const output = await reciprocal.getOutput()
```

#### Trigonometric

Trigonometric functions (sin, cos, tan). Extends Transformer.

```typescript
const trigonometric = await dsp.createTrigonometric({ space: Space.TIME })
await trigonometric.setNumChannels(2)

// Inputs
const input = await trigonometric.getInput()

// Outputs
const output = await trigonometric.getOutput()

// Miscellaneous
await trigonometric.getMode()
await trigonometric.setMode(TrigonometricMode.SINE)
```

### Trigger Nodes

#### ClockTrigger

Generates periodic trigger signals. Extends Producer.

```typescript
const clockTrigger = await dsp.createClockTrigger()
await clockTrigger.setNumChannels(2)

// Inputs
const interval = await clockTrigger.getInterval()
const delayTime = await clockTrigger.getDelayTime()
const reset = await clockTrigger.getReset()

// Outputs
const output = await clockTrigger.getOutput()

// Miscellaneous
const currentTime = await clockTrigger.getCurrentTime()
```

#### Differentiator

Outputs trigger on input change. Extends Transformer.

```typescript
const differentiator = await dsp.createDifferentiator({ type: Type.RATIO })
await differentiator.setNumChannels(2)

// Inputs
const input = await differentiator.getInput()
const reset = await differentiator.getReset()

// Outputs
const output = await differentiator.getOutput()
const gate = await differentiator.getGate()
```

#### Integrator

Accumulates input while gate is high. Extends Transformer.

```typescript
const integrator = await dsp.createIntegrator({ type: Type.RATIO })
await integrator.setNumChannels(2)

// Inputs
const input = await integrator.getInput()
const gate = await integrator.getGate()
const reset = await integrator.getReset()

// Outputs
const output = await integrator.getOutput()
```

#### OnOff

Toggle switch with separate on/off triggers. Extends Producer.

```typescript
const onOff = await dsp.createOnOff()
await onOff.setNumChannels(2)

// Inputs
const onTrigger = await onOff.getOnTrigger()
const offTrigger = await onOff.getOffTrigger()

// Outputs
const output = await onOff.getOutput()
```

#### ResetTrigger

Generates a one-shot reset trigger. Extends Producer.

```typescript
const resetTrigger = await dsp.createResetTrigger()
await resetTrigger.setNumChannels(2)

// Outputs
const output = await resetTrigger.getOutput()

// Miscellaneous
await resetTrigger.reset()
await resetTrigger.resetChannel(0)
```

#### SampleAndHold

Samples input when triggered. Extends Transformer.

```typescript
const sampleAndHold = await dsp.createSampleAndHold({ type: Type.RATIO })
await sampleAndHold.setNumChannels(2)

// Inputs
const input = await sampleAndHold.getInput()
const gate = await sampleAndHold.getGate()
const reset = await sampleAndHold.getReset()

// Outputs
const output = await sampleAndHold.getOutput()
```

#### Sequencer

Step sequencer with multiple sequences. Extends Producer.

```typescript
const sequencer = await dsp.createSequencer({
  type: Type.RATIO,
  space: Space.TIME,
})
await sequencer.setNumChannels(2)

// Inputs
const sequenceIndex = await sequencer.getSequenceIndex()
const positionIndex = await sequencer.getPositionIndex()

// Outputs
const output = await sequencer.getOutput()

// Miscellaneous
const sequences = await sequencer.getSequences()
```

#### TriggerHold

Extends trigger duration. Extends Transformer.

```typescript
const triggerHold = await dsp.createTriggerHold()
await triggerHold.setNumChannels(2)

// Inputs
const input = await triggerHold.getInput()
const holdTime = await triggerHold.getHoldTime()

// Outputs
const output = await triggerHold.getOutput()

// Miscellaneous
const currentTime = await triggerHold.getCurrentTime()
```

### Variable Nodes

#### BufferDuration

Outputs the duration of one buffer in seconds. Extends Producer.

```typescript
const bufferDuration = await dsp.createBufferDuration()
await bufferDuration.setNumChannels(2)

// Outputs
const output = await bufferDuration.getOutput()
```

#### BufferRate

Outputs the buffer rate (buffers per second). Extends Producer.

```typescript
const bufferRate = await dsp.createBufferRate()
await bufferRate.setNumChannels(2)

// Outputs
const output = await bufferRate.getOutput()
```

#### SampleDuration

Outputs the duration of one sample in seconds. Extends Producer.

```typescript
const sampleDuration = await dsp.createSampleDuration()
await sampleDuration.setNumChannels(2)

// Outputs
const output = await sampleDuration.getOutput()
```

#### SampleRate

Outputs the sample rate in Hz. Extends Producer.

```typescript
const sampleRate = await dsp.createSampleRate()
await sampleRate.setNumChannels(2)

// Outputs
const output = await sampleRate.getOutput()
```

## MIDI Support

### MIDI Buffers

MIDI buffers store timestamped MIDI events. Each event contains a MIDI message and its sample position within the buffer.

```typescript
const midiBuffer = await dsp.createMidiBuffer()

const events = await midiBuffer.getEvents()
for (const event of events) {
  const message = event.message
  const samplePosition = event.samplePosition
}

await midiBuffer.addEvent(midiMessage, samplePosition)
await midiBuffer.addEvents(
  otherMidiBuffer,
  startSample,
  numSamples,
  sampleDeltaToAdd,
)
await midiBuffer.clear()
await midiBuffer.delete()
```

### MIDI Messages

Create MIDI messages using static factory methods on the DSP class:

```typescript
const noteOn = await dsp.createNoteOn(channel, noteNumber, velocity)
const noteOff = await dsp.createNoteOff(channel, noteNumber, velocity)
const aftertouch = await dsp.createAftertouch(
  channel,
  noteNumber,
  aftertouchValue,
)
const controller = await dsp.createController(
  channel,
  controllerNumber,
  controllerValue,
)
const programChange = await dsp.createProgramChange(channel, programNumber)
const channelPressure = await dsp.createChannelPressure(
  channel,
  channelPressureValue,
)
const pitchWheel = await dsp.createPitchWheel(channel, pitchWheelValue)
const allNotesOff = await dsp.createAllNotesOff(channel)
const songPositionPointer = await dsp.createSongPositionPointer(
  songPositionPointerMidiBeat,
)
const midiClock = await dsp.createMidiClock()
const midiStart = await dsp.createMidiStart()
const midiContinue = await dsp.createMidiContinue()
const midiStop = await dsp.createMidiStop()
const midiMessage = await dsp.createMidiMessage([0x90, 0x3c, 0x7f])
```

Query MIDI message properties:

```typescript
const isNote = await message.isNote()
const isNoteOff = await message.isNoteOff()
const isNoteOn = await message.isNoteOn()
const isAftertouch = await message.isAftertouch()
const isController = await message.isController()
const isProgramChange = await message.isProgramChange()
const isChannelPressure = await message.isChannelPressure()
const isPitchWheel = await message.isPitchWheel()
const isAllNotesOff = await message.isAllNotesOff()
const isSongPositionPointer = await message.isSongPositionPointer()
const isMidiClock = await message.isMidiClock()
const isMidiStart = await message.isMidiStart()
const isMidiContinue = await message.isMidiContinue()
const isMidiStop = await message.isMidiStop()

const channel = await message.getChannel()
const noteNumber = await message.getNoteNumber()
const velocity = await message.getVelocity()
const aftertouchValue = await message.getAftertouchValue()
const controllerNumber = await message.getControllerNumber()
const controllerValue = await message.getControllerValue()
const programChangeNumber = await message.getProgramChangeNumber()
const channelPressureValue = await message.getChannelPressureValue()
const pitchWheelValue = await message.getPitchWheelValue()
const songPositionPointerMidiBeat =
  await message.getSongPositionPointerMidiBeat()

const bytes = await message.getBytes()
const rawData = await message.getRawData()
const rawDataSize = await message.getRawDataSize()

await message.delete()
```

## FFT Support

The library provides FFT nodes for real-time processing and a standalone NormalizedFFT utility for offline analysis.

### FFT Nodes

Use ForwardFFT and InverseFFT nodes within the audio graph:

```typescript
const fft = await dsp.createForwardFFT()
const ifft = await dsp.createInverseFFT()

// Outputs
const magnitudeOutput = await fft.getMagnitude()
const phaseOutput = await fft.getPhase()

// Inputs
const magnitudeInput = await ifft.getMagnitude()
const phaseInput = await ifft.getPhase()
```

### NormalizedFFT

The NormalizedFFT utility provides standalone FFT operations for offline processing:

```typescript
const normalizedFFT = await dsp.createNormalizedFFT()
await normalizedFFT.setup(2048)

const size = await normalizedFFT.getSize()
const complexSize = await normalizedFFT.getComplexSize()

const timeData = [
  /* 2048 samples */
]
const { real, imaginary } = await normalizedFFT.toRealImaginary(timeData)
const { magnitude, phase } = await normalizedFFT.toMagnitudePhase(timeData)

const timeFromRI = await normalizedFFT.fromRealImaginary(real, imaginary)
const timeFromMP = await normalizedFFT.fromMagnitudePhase(magnitude, phase)
```

## Enums

### Global Enums

#### Type

Signal value type.

```typescript
import { Type } from '@potrepka/react-native-dsp'

Type.RATIO // Normalized ratio (-1 to 1 or 0 to 1)
Type.INTEGER // Integer values
Type.BOOLEAN // Boolean (0 or 1)
Type.HERTZ // Frequency in Hz
Type.SECONDS // Time in seconds
```

#### Space

Signal processing domain.

```typescript
import { Space } from '@potrepka/react-native-dsp'

Space.TIME // Time domain
Space.FREQUENCY // Frequency domain
```

#### Shape

Curve shape for envelope attack and release phases. Linear curves change at a constant rate, while exponential curves change more rapidly at the start and slow down toward the end.

```typescript
import { Shape } from '@potrepka/react-native-dsp'

Shape.LINEAR // Linear curve (constant rate of change)
Shape.EXPONENTIAL // Exponential curve (faster initial change)
```

#### Interpolation

Interpolation method for sample lookups.

```typescript
import { Interpolation } from '@potrepka/react-native-dsp'

Interpolation.NONE // No interpolation
Interpolation.LINEAR // Linear interpolation
Interpolation.HERMITE // Hermite interpolation
```

#### InputMode

Input summing mode for combining multiple connections.

```typescript
import { InputMode } from '@potrepka/react-native-dsp'

InputMode.SUM // Sum all connected inputs
InputMode.MINIMUM // Use minimum of all connected inputs
InputMode.MAXIMUM // Use maximum of all connected inputs
```

### Node Enums

#### RecorderMode

```typescript
import { RecorderMode } from '@potrepka/react-native-dsp'

RecorderMode.FIXED_SINGLE // Fixed length, single buffer
RecorderMode.FIXED_DOUBLE // Fixed length, double buffer
RecorderMode.VARIABLE // Variable length
```

#### SpreadMode

```typescript
import { SpreadMode } from '@potrepka/react-native-dsp'

SpreadMode.UNIPOLAR // 0 to 1 range
SpreadMode.BIPOLAR // -1 to 1 range
```

#### ClipperMode

```typescript
import { ClipperMode } from '@potrepka/react-native-dsp'

ClipperMode.CLIP // Hard clip
ClipperMode.WRAP // Wrap around
ClipperMode.FOLD // Fold back
```

#### ShaperMode

```typescript
import { ShaperMode } from '@potrepka/react-native-dsp'

ShaperMode.HYPERBOLIC // Hyperbolic tangent
ShaperMode.RATIONAL // Rational function
```

#### BiquadMode

```typescript
import { BiquadMode } from '@potrepka/react-native-dsp'

BiquadMode.LOW_PASS // Low-pass filter
BiquadMode.HIGH_PASS // High-pass filter
BiquadMode.BAND_PASS // Band-pass filter
BiquadMode.BAND_STOP // Band-reject filter
BiquadMode.LOW_SHELF // Low shelf EQ
BiquadMode.HIGH_SHELF // High shelf EQ
BiquadMode.PEAK // Peaking EQ
BiquadMode.ALL_PASS // All-pass filter
```

#### OnePoleMode

```typescript
import { OnePoleMode } from '@potrepka/react-native-dsp'

OnePoleMode.LOW_PASS // Low-pass filter
OnePoleMode.HIGH_PASS // High-pass filter
```

#### NoiseMode

```typescript
import { NoiseMode } from '@potrepka/react-native-dsp'

NoiseMode.WHITE // White noise
NoiseMode.PINK // Pink noise
```

#### PhasorMode

```typescript
import { PhasorMode } from '@potrepka/react-native-dsp'

PhasorMode.WRAPPED // Wrap at 1.0
PhasorMode.UNBOUNDED // Continue accumulating
```

#### ComparisonMode

```typescript
import { ComparisonMode } from '@potrepka/react-native-dsp'

ComparisonMode.EQUAL // Output 1 if input == threshold, else 0
ComparisonMode.NOT_EQUAL // Output 1 if input != threshold, else 0
ComparisonMode.LESS_THAN // Output 1 if input < threshold, else 0
ComparisonMode.LESS_THAN_OR_EQUAL // Output 1 if input <= threshold, else 0
ComparisonMode.GREATER_THAN // Output 1 if input > threshold, else 0
ComparisonMode.GREATER_THAN_OR_EQUAL // Output 1 if input >= threshold, else 0
```

#### HyperbolicMode

```typescript
import { HyperbolicMode } from '@potrepka/react-native-dsp'

HyperbolicMode.SINE // Hyperbolic sine
HyperbolicMode.COSINE // Hyperbolic cosine
HyperbolicMode.TANGENT // Hyperbolic tangent
```

#### TrigonometricMode

```typescript
import { TrigonometricMode } from '@potrepka/react-native-dsp'

TrigonometricMode.SINE // Sine function
TrigonometricMode.COSINE // Cosine function
TrigonometricMode.TANGENT // Tangent function
```

## Utility Functions

The DSP class provides utility functions for common operations:

```typescript
const unipolar = await dsp.byteToUnipolar(128)
const byte = await dsp.unipolarToByte(0.5)
const bipolar = await dsp.shortToBipolar(16384)
const short = await dsp.bipolarToShort(0.5)
const clipped = await dsp.clip(1.5, -1, 1)
const wrapped = await dsp.wrap(1.5, 1)
const lerp = await dsp.linear(0, 1, 0.5)
const hermite = await dsp.hermite(0, 0, 1, 1, 0.5)
const linearSample = await dsp.linearClipped(data, 1.5, 0)
const hermiteSample = await dsp.hermiteClipped(data, 1.5, 0)
const linearWrapped = await dsp.linearWrapped(data, 1.5, 0)
const hermiteWrapped = await dsp.hermiteWrapped(data, 1.5, 0)
```

## Constants

Mathematical and physical constants:

```typescript
import {
  PI, // 3.141592653589793
  ONE_OVER_PI, // 0.3183098861837907
  TAU, // 6.283185307179586
  ONE_OVER_TAU, // 0.1591549430918953
  PI_OVER_TWO, // 1.570796326794897
  SQRT_OF_TWO, // 1.414213562373095
  LOG_OF_TWO, // 0.6931471805599453
  ONE_OVER_LOG_OF_TWO, // 1.442695040888963
  PHI, // 1.618033988749895
  E, // 2.718281828459045
} from '@potrepka/react-native-dsp'
```

## Platform Detection

Detect the current platform:

```typescript
import { Platform, getPlatformInfo } from '@potrepka/react-native-dsp'

console.log(Platform.isWeb) // true on web
console.log(Platform.isNative) // true on React Native
console.log(Platform.name) // 'web', 'ios', 'android', or 'unknown'

const info = getPlatformInfo()
```

## License

MIT
