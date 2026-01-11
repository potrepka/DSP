export { BaseProxy } from './classes/proxy/core/BaseProxy'
export {
  BufferProxy,
  InputProxy,
  OutputProxy,
} from './classes/proxy/core/BufferProxy'
export { ConsumerProxy } from './classes/proxy/core/ConsumerProxy'
export { DataProxy } from './classes/proxy/core/DataProxy'
export { LockableProxy } from './classes/proxy/core/LockableProxy'
export { NodeProcessorProxy } from './classes/proxy/core/NodeProcessorProxy'
export { NodeProxy } from './classes/proxy/core/NodeProxy'
export { NormalizedFFTProxy } from './classes/proxy/core/NormalizedFFTProxy'
export { ProducerProxy } from './classes/proxy/core/ProducerProxy'
export { TransformerProxy } from './classes/proxy/core/TransformerProxy'
export { VectorProxy } from './classes/proxy/core/VectorProxy'
export { WrapperProxy } from './classes/proxy/core/WrapperProxy'
export { DSP } from './classes/proxy/DSP'
export { MidiBufferProxy } from './classes/proxy/midi/MidiBufferProxy'
export { MidiMessageProxy } from './classes/proxy/midi/MidiMessageProxy'
export { RecorderProxy } from './classes/proxy/nodes/analyzers/RecorderProxy'
export { ChannelMergerProxy } from './classes/proxy/nodes/channel/ChannelMergerProxy'
export { ChannelSplitterProxy } from './classes/proxy/nodes/channel/ChannelSplitterProxy'
export { MidSideProxy } from './classes/proxy/nodes/channel/MidSideProxy'
export { SpreadProxy } from './classes/proxy/nodes/channel/SpreadProxy'
export { StereoPannerProxy } from './classes/proxy/nodes/channel/StereoPannerProxy'
export { ConvolverProxy } from './classes/proxy/nodes/delays/ConvolverProxy'
export { VariableDelayProxy } from './classes/proxy/nodes/delays/VariableDelayProxy'
export { ClipperProxy } from './classes/proxy/nodes/dynamics/ClipperProxy'
export { CompressorGateProxy } from './classes/proxy/nodes/dynamics/CompressorGateProxy'
export { DryWetProxy } from './classes/proxy/nodes/dynamics/DryWetProxy'
export { EnvelopeProxy } from './classes/proxy/nodes/dynamics/EnvelopeProxy'
export { LagProxy } from './classes/proxy/nodes/dynamics/LagProxy'
export { ShaperProxy } from './classes/proxy/nodes/dynamics/ShaperProxy'
export { TableShaperProxy } from './classes/proxy/nodes/dynamics/TableShaperProxy'
export { MidiInputProxy } from './classes/proxy/nodes/external/MidiInputProxy'
export { MidiOutputProxy } from './classes/proxy/nodes/external/MidiOutputProxy'
export { BiquadProxy } from './classes/proxy/nodes/filters/BiquadProxy'
export { CrossoverProxy } from './classes/proxy/nodes/filters/CrossoverProxy'
export { OnePoleProxy } from './classes/proxy/nodes/filters/OnePoleProxy'
export { NoiseProxy } from './classes/proxy/nodes/generators/NoiseProxy'
export { PhasorProxy } from './classes/proxy/nodes/generators/PhasorProxy'
export { SamplePlayerProxy } from './classes/proxy/nodes/generators/SamplePlayerProxy'
export { TableOscillatorProxy } from './classes/proxy/nodes/generators/TableOscillatorProxy'
export { AbsoluteValueProxy } from './classes/proxy/nodes/math/AbsoluteValueProxy'
export { BooleanMaskProxy } from './classes/proxy/nodes/math/BooleanMaskProxy'
export { ComparisonProxy } from './classes/proxy/nodes/math/ComparisonProxy'
export { DivisionProxy } from './classes/proxy/nodes/math/DivisionProxy'
export { FloorProxy } from './classes/proxy/nodes/math/FloorProxy'
export { ForwardFFTProxy } from './classes/proxy/nodes/math/ForwardFFTProxy'
export { FrequencyToNoteProxy } from './classes/proxy/nodes/math/FrequencyToNoteProxy'
export { FunctionProxy } from './classes/proxy/nodes/math/FunctionProxy'
export { HyperbolicProxy } from './classes/proxy/nodes/math/HyperbolicProxy'
export { IdentityProxy } from './classes/proxy/nodes/math/IdentityProxy'
export { InverseFFTProxy } from './classes/proxy/nodes/math/InverseFFTProxy'
export { LogarithmProxy } from './classes/proxy/nodes/math/LogarithmProxy'
export { ModuloProxy } from './classes/proxy/nodes/math/ModuloProxy'
export { MultiplicationProxy } from './classes/proxy/nodes/math/MultiplicationProxy'
export { NegativeProxy } from './classes/proxy/nodes/math/NegativeProxy'
export { NoteToFrequencyProxy } from './classes/proxy/nodes/math/NoteToFrequencyProxy'
export { NotGateProxy } from './classes/proxy/nodes/math/NotGateProxy'
export { PowerProxy } from './classes/proxy/nodes/math/PowerProxy'
export { ReciprocalProxy } from './classes/proxy/nodes/math/ReciprocalProxy'
export { TrigonometricProxy } from './classes/proxy/nodes/math/TrigonometricProxy'
export { ClockTriggerProxy } from './classes/proxy/nodes/trigger/ClockTriggerProxy'
export { DifferentiatorProxy } from './classes/proxy/nodes/trigger/DifferentiatorProxy'
export { IntegratorProxy } from './classes/proxy/nodes/trigger/IntegratorProxy'
export { OnOffProxy } from './classes/proxy/nodes/trigger/OnOffProxy'
export { ResetTriggerProxy } from './classes/proxy/nodes/trigger/ResetTriggerProxy'
export { SampleAndHoldProxy } from './classes/proxy/nodes/trigger/SampleAndHoldProxy'
export { SequencerProxy } from './classes/proxy/nodes/trigger/SequencerProxy'
export { TriggerHoldProxy } from './classes/proxy/nodes/trigger/TriggerHoldProxy'
export { BufferDurationProxy } from './classes/proxy/nodes/variables/BufferDurationProxy'
export { BufferRateProxy } from './classes/proxy/nodes/variables/BufferRateProxy'
export { SampleDurationProxy } from './classes/proxy/nodes/variables/SampleDurationProxy'
export { SampleRateProxy } from './classes/proxy/nodes/variables/SampleRateProxy'
export {
  E,
  LOG_OF_TWO,
  ONE_OVER_LOG_OF_TWO,
  ONE_OVER_PI,
  ONE_OVER_TAU,
  PHI,
  PI,
  PI_OVER_TWO,
  SQRT_OF_TWO,
  TAU,
} from './constants/math'
export { ID_LENGTH, NODE_PROCESSOR_ID } from './constants/proxy'
export { Domain, InputMode, Interpolation, Shape, Type } from './enums/global'
export {
  BiquadMode,
  ClipperMode,
  HyperbolicMode,
  NoiseMode,
  OnePoleMode,
  PhasorMode,
  RecorderMode,
  ShaperMode,
  SpreadMode,
  TrigonometricMode,
} from './enums/node'
export { createObject } from './helpers/module'
export { getPlatformInfo, Platform } from './helpers/platform'
export { chainable } from './helpers/proxy'
export { initializeWebAudio } from './helpers/web'
export type { ComplexData, FFTData } from './types/fft'
export type {
  AbsoluteValue,
  AudioFFTSampleVector,
  AudioModule,
  Biquad,
  BooleanMask,
  Buffer,
  BufferDuration,
  BufferRate,
  BufferVector,
  ChannelMerger,
  ChannelSplitter,
  Clipper,
  ClockTrigger,
  Comparison,
  CompressorGate,
  Consumer,
  Convolver,
  CoreConsturctorMap,
  Crossover,
  Data,
  Deletable,
  Differentiator,
  Division,
  DryWet,
  Engine,
  Envelope,
  Floor,
  ForwardFFT,
  FrequencyToNote,
  Function,
  GlobalFunctionMap,
  Hyperbolic,
  Identity,
  Input,
  InputVector,
  Integrator,
  InverseFFT,
  Lag,
  Lockable,
  Logarithm,
  MidiBuffer,
  MidiConstructorMap,
  MidiInput,
  MidiMessage,
  MidiOutput,
  MidiProcessor,
  MidiProcessorInput,
  MidiProcessorInputVector,
  MidiProcessorOutput,
  MidiProcessorOutputVector,
  MidSide,
  Modulo,
  Multiplication,
  Negative,
  Node,
  NodeConstructorMap,
  NodeProcessor,
  NodeProcessorOptions,
  NodeType,
  NodeVector,
  Noise,
  NormalizedFFT,
  NoteToFrequency,
  NotGate,
  ObjectConstructorMap,
  ObjectType,
  OnePole,
  OnOff,
  Options,
  Output,
  OutputVector,
  Phasor,
  Power,
  Producer,
  Reciprocal,
  Recorder,
  RequestMessage,
  ResetTrigger,
  ResponseMessage,
  SampleAndHold,
  SampleDuration,
  SamplePlayer,
  SampleRate,
  SampleVector,
  Sequencer,
  SerializedValue,
  Shaper,
  Spread,
  StereoPanner,
  TableOscillator,
  TableShaper,
  Target,
  Transformer,
  TriggerHold,
  Trigonometric,
  UInt8Vector,
  UIntVector,
  VariableDelay,
  Vector,
  VectorConstructorMap,
  Wrapper,
} from './types/module'
export type { PlatformInfo, PlatformName } from './types/platform'
export type { Chainable, ProxyContext } from './types/proxy'
