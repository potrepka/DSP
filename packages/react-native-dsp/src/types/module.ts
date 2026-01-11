import { Domain, InputMode, Interpolation, Shape, Type } from '../enums/global'
import {
  BiquadMode,
  ClipperMode,
  ComparisonMode,
  HyperbolicMode,
  NoiseMode,
  OnePoleMode,
  PhasorMode,
  RecorderMode,
  ShaperMode,
  SpreadMode,
  TrigonometricMode,
} from '../enums/node'

export type AudioModule = GlobalFunctionMap & ObjectConstructorMap

export type GlobalFunctionMap = {
  byteToUnipolar: (value: number) => number
  unipolarToByte: (sample: number) => number
  shortToBipolar: (value: number) => number
  bipolarToShort: (sample: number) => number
  clip: (sample: number, min: number, max: number) => number
  wrap: (sample: number, max: number) => number
  linear: (x1: number, x2: number, mu: number) => number
  hermite: (
    x0: number,
    x1: number,
    x2: number,
    x3: number,
    mu: number,
  ) => number
  linearClipped: (data: number[], index: number, defaultValue: number) => number
  hermiteClipped: (
    data: number[],
    index: number,
    defaultValue: number,
  ) => number
  linearWrapped: (data: number[], index: number, defaultValue: number) => number
  hermiteWrapped: (
    data: number[],
    index: number,
    defaultValue: number,
  ) => number
}

export type ObjectConstructorMap = VectorConstructorMap &
  CoreConsturctorMap &
  MidiConstructorMap &
  NodeConstructorMap

export type ObjectType = keyof ObjectConstructorMap

export type VectorConstructorMap = {
  UInt8Vector: new () => UInt8Vector
  UIntVector: new () => UIntVector
  SampleVector: new () => SampleVector
  AudioFFTSampleVector: new () => AudioFFTSampleVector
  BufferVector: new () => BufferVector
  InputVector: new () => InputVector
  OutputVector: new () => OutputVector
  NodeVector: new () => NodeVector
  MidiProcessorInputVector: new () => MidiProcessorInputVector
  MidiProcessorOutputVector: new () => MidiProcessorOutputVector
}

export type CoreConsturctorMap = {
  Data: new (numChannels: number, numSamples: number) => Data
  Wrapper: {
    new (): Wrapper
    new (data: Data): Wrapper
  }
  Buffer: new (
    type: Type,
    domain: Domain,
    range: number,
    defaultValue: number,
    numChannels: number,
    numSamples: number,
  ) => Buffer
  Input: new (
    type: Type,
    domain: Domain,
    range: number,
    defaultValue: number,
    numChannels: number,
    numSamples: number,
  ) => Input
  Output: new (
    type: Type,
    domain: Domain,
    range: number,
    defaultValue: number,
    numChannels: number,
    numSamples: number,
  ) => Output
  Lockable: new () => Lockable
  Engine: new () => Engine
  Node: new () => Node
  Consumer: new (type: Type, domain: Domain) => Consumer
  Producer: new (type: Type, domain: Domain) => Producer
  Transformer: {
    new (type: Type, domain: Domain): Transformer
    new (inputType: Type, outputType: Type, domain: Domain): Transformer
    new (
      inputType: Type,
      outputType: Type,
      inputDomain: Domain,
      outputDomain: Domain,
    ): Transformer
  }
  NodeProcessor: new (
    numInputChannels: number,
    numOutputChannels: number,
    numSamples: number,
    sampleRate: number,
  ) => NodeProcessor
  NormalizedFFT: new () => NormalizedFFT
}

export type MidiConstructorMap = {
  MidiBuffer: new () => MidiBuffer
  MidiMessage: {
    new (byte0: number): MidiMessage
    new (byte0: number, byte1: number): MidiMessage
    new (byte0: number, byte1: number, byte2: number): MidiMessage
    fromArray: (bytes: UInt8Vector) => MidiMessage
    noteOff: (
      channel: number,
      noteNumber: number,
      velocity: number,
    ) => MidiMessage
    noteOn: (
      channel: number,
      noteNumber: number,
      velocity: number,
    ) => MidiMessage
    aftertouch: (
      channel: number,
      noteNumber: number,
      aftertouchValue: number,
    ) => MidiMessage
    controller: (
      channel: number,
      controllerNumber: number,
      controllerValue: number,
    ) => MidiMessage
    programChange: (channel: number, programNumber: number) => MidiMessage
    channelPressure: (
      channel: number,
      channelPressureValue: number,
    ) => MidiMessage
    pitchWheel: (channel: number, pitchWheelValue: number) => MidiMessage
    allNotesOff: (channel: number) => MidiMessage
    songPositionPointer: (songPositionPointerMidiBeat: number) => MidiMessage
    midiClock: () => MidiMessage
    midiStart: () => MidiMessage
    midiContinue: () => MidiMessage
    midiStop: () => MidiMessage
  }
  MidiProcessor: new () => MidiProcessor
  MidiProcessorInput: new (port: number) => MidiProcessorInput
  MidiProcessorOutput: new (port: number) => MidiProcessorOutput
}

export type NodeConstructorMap = {
  // Analyzer Nodes
  Recorder: new (type: Type, domain: Domain, defaultValue: number) => Recorder

  // Channel Nodes
  ChannelMerger: new (type: Type, domain: Domain) => ChannelMerger
  ChannelSplitter: new (type: Type, domain: Domain) => ChannelSplitter
  MidSide: new (type: Type, domain: Domain) => MidSide
  Spread: new (type: Type, domain: Domain) => Spread
  StereoPanner: new (type: Type, domain: Domain) => StereoPanner

  // Delay Nodes
  Convolver: new () => Convolver
  VariableDelay: new (type: Type) => VariableDelay

  // Dynamics Nodes
  Clipper: new (type: Type, domain: Domain) => Clipper
  CompressorGate: new () => CompressorGate
  DryWet: new (type: Type, domain: Domain) => DryWet
  Envelope: new () => Envelope
  Lag: new (type: Type) => Lag
  Shaper: new (domain: Domain) => Shaper
  TableShaper: new (outputType: Type, domain: Domain) => TableShaper

  // External Nodes
  MidiInput: new (midiBuffer: MidiBuffer, type: Type) => MidiInput
  MidiOutput: new (midiBuffer: MidiBuffer, type: Type) => MidiOutput

  // Filter Nodes
  Biquad: new () => Biquad
  Crossover: new () => Crossover
  OnePole: new (type: Type) => OnePole

  // Generator Nodes
  Noise: new () => Noise
  Phasor: new () => Phasor
  SamplePlayer: new (type: Type) => SamplePlayer
  TableOscillator: new (type: Type) => TableOscillator

  // Math Nodes
  AbsoluteValue: new (type: Type, domain: Domain) => AbsoluteValue
  BooleanMask: new (type: Type, domain: Domain) => BooleanMask
  Comparison: new (type: Type, domain: Domain) => Comparison
  Division: new (type: Type, domain: Domain) => Division
  Floor: new (type: Type, domain: Domain) => Floor
  ForwardFFT: new () => ForwardFFT
  FrequencyToNote: new (domain: Domain) => FrequencyToNote
  Function: {
    new (type: Type, domain: Domain): Function
    new (aType: Type, bType: Type, outputType: Type, domain: Domain): Function
  }
  Hyperbolic: new (domain: Domain) => Hyperbolic
  Identity: {
    new (type: Type, domain: Domain): Identity
    new (inputType: Type, outputType: Type, domain: Domain): Identity
    new (
      inputType: Type,
      outputType: Type,
      inputDomain: Domain,
      outputDomain: Domain,
    ): Identity
  }
  InverseFFT: new () => InverseFFT
  Logarithm: new (domain: Domain) => Logarithm
  Modulo: new (type: Type, domain: Domain) => Modulo
  Multiplication: new (type: Type, domain: Domain) => Multiplication
  Negative: new (type: Type, domain: Domain) => Negative
  NoteToFrequency: new (domain: Domain) => NoteToFrequency
  NotGate: new (domain: Domain) => NotGate
  Power: new (domain: Domain) => Power
  Reciprocal: new (type: Type, domain: Domain) => Reciprocal
  Trigonometric: new (domain: Domain) => Trigonometric

  // Trigger Nodes
  ClockTrigger: new () => ClockTrigger
  Differentiator: new (type: Type) => Differentiator
  Integrator: new (type: Type) => Integrator
  OnOff: new () => OnOff
  ResetTrigger: new () => ResetTrigger
  SampleAndHold: new (type: Type) => SampleAndHold
  Sequencer: new (type: Type, domain: Domain) => Sequencer
  TriggerHold: new () => TriggerHold

  // Variable Nodes
  BufferDuration: new () => BufferDuration
  BufferRate: new () => BufferRate
  SampleDuration: new () => SampleDuration
  SampleRate: new () => SampleRate
}

export type NodeType = keyof NodeConstructorMap

type OptionsMap = {
  // Vectors
  UInt8Vector: unknown
  UIntVector: unknown
  SampleVector: unknown
  AudioFFTSampleVector: unknown
  BufferVector: unknown
  InputVector: unknown
  OutputVector: unknown
  NodeVector: unknown
  MidiProcessorInputVector: unknown
  MidiProcessorOutputVector: unknown

  // Core objects
  Data: { numChannels: number; numSamples: number }
  Wrapper: { data?: Data }
  Buffer: {
    type: Type
    domain: Domain
    range: number
    defaultValue: number
    numChannels: number
    numSamples: number
  }
  Input: {
    type: Type
    domain: Domain
    range: number
    defaultValue: number
    numChannels: number
    numSamples: number
  }
  Output: {
    type: Type
    domain: Domain
    range: number
    defaultValue: number
    numChannels: number
    numSamples: number
  }
  Lockable: unknown
  Engine: unknown
  Node: unknown
  Consumer: { type: Type; domain: Domain }
  Producer: { type: Type; domain: Domain }
  Transformer: {
    type?: Type
    domain?: Domain
    inputType?: Type
    outputType?: Type
    inputDomain?: Domain
    outputDomain?: Domain
  }
  NodeProcessor: {
    numInputChannels: number
    numOutputChannels: number
    numSamples: number
    sampleRate: number
  }
  NormalizedFFT: unknown

  // MIDI objects
  MidiBuffer: unknown
  MidiMessage: { byte0: number; byte1: number; byte2: number }
  MidiProcessor: unknown
  MidiProcessorInput: { port: number }
  MidiProcessorOutput: { port: number }

  // Nodes - Analyzer
  Recorder: {
    type: Type
    domain: Domain
    defaultValue: number
  }

  // Nodes - Channel
  ChannelMerger: { type: Type; domain: Domain }
  ChannelSplitter: { type: Type; domain: Domain }
  MidSide: { type: Type; domain: Domain }
  Spread: { type: Type; domain: Domain }
  StereoPanner: { type: Type; domain: Domain }

  // Nodes - Delay
  Convolver: unknown
  VariableDelay: { type: Type }

  // Nodes - Dynamics
  Clipper: { type: Type; domain: Domain }
  CompressorGate: unknown
  DryWet: { type: Type; domain: Domain }
  Envelope: unknown
  Lag: { type: Type }
  Shaper: { domain: Domain }
  TableShaper: { outputType: Type; domain: Domain }

  // Nodes - External
  MidiInput: { midiBuffer: MidiBuffer; type: Type }
  MidiOutput: { midiBuffer: MidiBuffer; type: Type }

  // Nodes - Filter
  Biquad: unknown
  Crossover: unknown
  OnePole: { type: Type }

  // Nodes - Generator
  Noise: unknown
  Phasor: unknown
  SamplePlayer: { type: Type }
  TableOscillator: { type: Type }

  // Nodes - Math
  AbsoluteValue: { type: Type; domain: Domain }
  BooleanMask: { type: Type; domain: Domain }
  Comparison: { type: Type; domain: Domain }
  Division: { type: Type; domain: Domain }
  Floor: { type: Type; domain: Domain }
  ForwardFFT: unknown
  FrequencyToNote: { domain: Domain }
  Function: {
    type: Type
    domain: Domain
    aType?: Type
    bType?: Type
    outputType?: Type
  }
  Hyperbolic: { domain: Domain }
  Identity: {
    type: Type
    domain: Domain
    inputType?: Type
    outputType?: Type
    inputDomain?: Domain
    outputDomain?: Domain
  }
  InverseFFT: unknown
  Logarithm: { domain: Domain }
  Modulo: { type: Type; domain: Domain }
  Multiplication: { type: Type; domain: Domain }
  Negative: { type: Type; domain: Domain }
  NoteToFrequency: { domain: Domain }
  NotGate: { domain: Domain }
  Power: { domain: Domain }
  Reciprocal: { type: Type; domain: Domain }
  Trigonometric: { domain: Domain }

  // Nodes - Trigger
  ClockTrigger: unknown
  Differentiator: { type: Type }
  Integrator: { type: Type }
  OnOff: unknown
  ResetTrigger: unknown
  SampleAndHold: { type: Type }
  Sequencer: { type: Type; domain: Domain }
  TriggerHold: unknown

  // Nodes - Variable
  BufferDuration: unknown
  BufferRate: unknown
  SampleDuration: unknown
  SampleRate: unknown
}

export type Options<T extends ObjectType> = Partial<OptionsMap[T]>

export type NodeProcessorOptions = {
  numInputChannels: number
  numOutputChannels: number
  numSamples: number
  sampleRate: number
}

export type Target = {
  __type: 'Target'
  id: string
  isChild?: boolean
}

export type SerializedValue =
  | null
  | string
  | boolean
  | number
  | bigint
  | SerializedValue[]
  | Target

export type RequestMessage<T extends ObjectType> =
  | {
      message: 'createObject'
      requestId: string
      objectType: T
      options?: Options<T>
    }
  | {
      message: 'deleteObject'
      requestId: string
      objectId: string
    }
  | {
      message: 'callStatic'
      requestId: string
      objectType: string
      methodName: string
      args: SerializedValue[]
    }
  | {
      message: 'callFunction'
      requestId: string
      functionName: string
      args: SerializedValue[]
    }
  | {
      message: 'call'
      requestId: string
      target: Target
      methodName: string
      args: SerializedValue[]
    }
  | {
      message: 'delete'
    }

export type ResponseMessage =
  | {
      message: 'state'
      state: 'running' | 'closed'
    }
  | {
      message: 'response'
      requestId: string
      result?: SerializedValue
      error?: string
    }

export type Deletable = {
  delete: () => void
}

export type Vector<T> = Deletable & {
  clone: () => Vector<T>
  get: (index: number) => T
  isAliasOf: (other: Vector<T>) => boolean
  push_back: (value: T) => void
  resize: (count: number, value: T) => void
  set: (index: number, value: T) => boolean
  size: () => number
}

export type UInt8Vector = Vector<number>
export type UIntVector = Vector<number>
export type SampleVector = Vector<number>
export type AudioFFTSampleVector = Vector<number>
export type BufferVector = Vector<Buffer>
export type InputVector = Vector<Input>
export type OutputVector = Vector<Output>
export type NodeVector = Vector<Node>
export type MidiProcessorInputVector = Vector<MidiProcessorInput>
export type MidiProcessorOutputVector = Vector<MidiProcessorOutput>

// ========== Core Classes ==========

export type Data = Deletable & {
  getNumChannels: () => number
  getNumSamples: () => number
  setSize: (numChannels: number, numSamples: number) => void
  clear: () => void
  getReadChannelData: (channel: number) => Float64Array
  getWriteChannelData: (channel: number) => Float64Array
  getPeakLevel: () => number
  getRMSLevel: () => number
  getReadData: () => Float64Array[]
  getWriteData: () => Float64Array[]
}

export type Wrapper = Deletable & {
  getNumChannels: () => number
  getNumSamples: () => number
  getChannelData: (channel: number) => Float64Array
  getSingleChannel: (channel: number) => Wrapper
  getSampleRange: (startSample: number, numSamples: number) => Wrapper
  clear: () => void
  fill: (value: number) => void
  apply: (f: (sample: number) => number) => void
  replaceWithApplicationOf: (
    f: (sample: number) => number,
    src: Wrapper,
  ) => Wrapper
  replaceWithApplicationOfTwoArgs: (
    f: (sample1: number, sample2: number) => number,
    src1: Wrapper,
    src2: Wrapper,
  ) => Wrapper
  copyFrom: (src: Wrapper) => void
  add: (value: number) => Wrapper
  addWrapper: (src: Wrapper) => Wrapper
  multiplyBy: (value: number) => Wrapper
  multiplyByWrapper: (src: Wrapper) => Wrapper
  addProductOf: (src: Wrapper, value: number) => Wrapper
  addProductOfWrappers: (src1: Wrapper, src2: Wrapper) => Wrapper
  replaceWithNegativeOf: (src: Wrapper) => Wrapper
  replaceWithAbsoluteValueOf: (src: Wrapper) => Wrapper
  replaceWithSumOf: (src: Wrapper, value: number) => Wrapper
  replaceWithSumOfWrappers: (src1: Wrapper, src2: Wrapper) => Wrapper
  replaceWithProductOf: (src: Wrapper, value: number) => Wrapper
  replaceWithProductOfWrappers: (src1: Wrapper, src2: Wrapper) => Wrapper
  replaceWithMinOf: (src: Wrapper, value: number) => Wrapper
  replaceWithMinOfWrappers: (src1: Wrapper, src2: Wrapper) => Wrapper
  replaceWithMaxOf: (src: Wrapper, value: number) => Wrapper
  replaceWithMaxOfWrappers: (src1: Wrapper, src2: Wrapper) => Wrapper
  getSample: (channel: number, sampleOffset: number) => number
  setSample: (channel: number, sampleOffset: number, value: number) => void
}

export type Buffer = Deletable & {
  getType: () => Type
  setType: (type: Type) => void
  getDomain: () => Domain
  setDomain: (domain: Domain) => void
  getRange: () => number
  setRange: (range: number) => void
  getDefaultValue: () => number
  setDefaultValue: (value: number) => void
  getNumChannels: () => number
  setNumChannels: (numChannels: number) => void
  getNumSamples: () => number
  setNumSamples: (numSamples: number) => void
  setSize: (numChannels: number, numSamples: number) => void
  getChannelValues: () => Float64Array
  setChannelValues: (values: Float64Array) => void
  getChannelValue: (channel: number) => number
  setChannelValue: (channel: number, value: number) => void
  setAllChannelValues: (value: number) => void
  getPeak: () => Float64Array
  getRMS: () => Float64Array
  getData: () => Data
  getWrapper: () => Wrapper
}

export type Input = Buffer & {
  getMode: () => InputMode
  setMode: (mode: InputMode) => void
  getConnections: () => OutputVector
  connect: (output: Output) => void
  disconnect: (output: Output) => void
  disconnectAll: () => void
  prepareNoLock: () => void
  processNoLock: () => void
}

export type Output = Buffer & {
  getConnections: () => InputVector
  connect: (input: Input) => void
  disconnect: (input: Input) => void
  disconnectAll: () => void
  prepareNoLock: () => void
  processNoLock: () => void
}

export type Lockable = Deletable & {
  lock: () => void
  unlock: () => void
}

export type Engine = Lockable & {
  getInputDevices: () => UIntVector
  getOutputDevices: () => UIntVector
  getSampleRates: (inputDevice: number, outputDevice: number) => UIntVector
  getDefaultInputDevice: () => number
  getDefaultOutputDevice: () => number
  getDefaultSampleRate: (inputDevice: number, outputDevice: number) => number
  setup: (
    inputDevice: number,
    outputDevice: number,
    numSamples: number,
    sampleRate: number,
  ) => void
  start: () => void
  getDeviceName: (device: number) => string
  getInputDeviceName: () => string
  getOutputDeviceName: () => string
  getNumInputChannels: () => number
  getNumOutputChannels: () => number
  getNumSamples: () => number
  getSampleRate: () => number
  getAudioBuffer: () => Data
  getNodeProcessor: () => NodeProcessor
  getMidiProcessor: () => MidiProcessor
}

export type Node = Deletable & {
  isActive: () => boolean
  setActive: (active: boolean) => void
  getNumChannels: () => number
  setNumChannels: (numChannels: number) => void
  getNumInputChannels: () => number
  setNumInputChannels: (numChannels: number) => void
  getNumOutputChannels: () => number
  setNumOutputChannels: (numChannels: number) => void
  getNumSamples: () => number
  setNumSamples: (numSamples: number) => void
  getSampleRate: () => number
  setSampleRate: (sampleRate: number) => void
  getOneOverNumSamples: () => number
  getOneOverSampleRate: () => number
  getInputs: () => InputVector
  getOutputs: () => OutputVector
  getChildren: () => NodeVector
  addChild: (child: Node) => void
  removeChild: (child: Node) => void
  sortChildren: () => void
  disconnectAll: () => void
  process: () => void
}

export type Consumer = Node & {
  getInput: () => Input
}

export type Producer = Node & {
  getOutput: () => Output
}

export type Transformer = Consumer & Producer

export type NodeProcessor = Deletable & {
  isActive: () => boolean
  setActive: (active: boolean) => void
  getAudioInput: () => Output
  getAudioOutput: () => Input
  getAudioInputClipping: () => Output
  getAudioOutputClipping: () => Output
  getNumInputChannels: () => number
  setNumInputChannels: (numChannels: number) => void
  getNumOutputChannels: () => number
  setNumOutputChannels: (numChannels: number) => void
  getNumSamples: () => number
  setNumSamples: (numSamples: number) => void
  getSampleRate: () => number
  setSampleRate: (sampleRate: number) => void
  setInputSize: (numChannels: number, numSamples: number) => void
  setOutputSize: (numChannels: number, numSamples: number) => void
  getDefaultNode: () => Node
  getNodes: () => NodeVector
  getInputMessages: () => MidiBuffer
  getOutputMessages: () => MidiBuffer
  process: (audioBuffer: Data, midibuffer: MidiBuffer) => void
}

// ========== Midi Classes ==========

export type MidiEvent = {
  samplePosition: number
  midiMessage: MidiMessage
}

export type MidiBuffer = Deletable & {
  getEvents: () => MidiEvent[]
  addEvent: (midiMessage: MidiMessage, sample: number) => void
  addEvents: (
    midiBuffer: MidiBuffer,
    startSample: number,
    numSamples: number,
    sampleDeltaToAdd: number,
  ) => void
  clear: () => void
}

export type MidiMessage = Deletable & {
  isNote: () => boolean
  isNoteOff: () => boolean
  isNoteOn: () => boolean
  isAftertouch: () => boolean
  isController: () => boolean
  isProgramChange: () => boolean
  isChannelPressure: () => boolean
  isPitchWheel: () => boolean
  isAllNotesOff: () => boolean
  isSongPositionPointer: () => boolean
  isMidiClock: () => boolean
  isMidiStart: () => boolean
  isMidiContinue: () => boolean
  isMidiStop: () => boolean
  getChannel: () => number
  getNoteNumber: () => number
  getVelocity: () => number
  getAftertouchValue: () => number
  getControllerNumber: () => number
  getControllerValue: () => number
  getProgramChangeNumber: () => number
  getChannelPressureValue: () => number
  getPitchWheelValue: () => number
  getSongPositionPointerMidiBeat: () => number
  getBytes: () => Uint8Array
  getRawData: () => Uint8Array
  getRawDataSize: () => number
}

export type MidiProcessor = Lockable & {
  getNumSamples: () => number
  setNumSamples: (numSamples: number) => void
  getSampleRate: () => number
  setSampleRate: (sampleRate: number) => void
  getMidiBuffer: () => MidiBuffer
  getInputs: () => MidiProcessorInputVector
  getOutputs: () => MidiProcessorOutputVector
  processInputs: () => void
  processOutputs: () => void
}

export type MidiProcessorInput = Lockable & {
  callback: (delta: number, bytes: Uint8Array) => void
  getDeviceName: () => string
  setPort: (port: number) => void
}

export type MidiProcessorOutput = Lockable & {
  getDeviceName: () => string
  setPort: (port: number) => void
  sendMessageWithDelay: (message: MidiMessage, delay: number) => void
}

// ========== Analyzer Nodes ==========

export type Recorder = Consumer & {
  getMode: () => RecorderMode
  setMode: (mode: RecorderMode) => void
  getRecordingTime: () => number
  setRecordingTime: (time: number) => void
  getRecordingBuffer: () => Buffer
  getGate: () => Input
  getReset: () => Input
}

// ========== Channel Nodes ==========

export type ChannelMerger = Producer & {
  getInput: (channel: number) => Input
}

export type ChannelSplitter = Consumer & {
  getOutput: (channel: number) => Output
}

export type MidSide = Consumer & {
  getMixAmount: () => Input
  getMid: () => Output
  getSide: () => Output
}

export type Spread = Transformer & {
  getMode: () => SpreadMode
  setMode: (mode: SpreadMode) => void
  getSpread: () => Input
}

export type StereoPanner = Consumer & {
  getDirection: () => Input
  getLeft: () => Output
  getRight: () => Output
}

// ========== Delay Nodes ==========

export type Convolver = Transformer & {
  getHeadSize: () => number
  setHeadSize: (headSize: number) => void
  getTailSize: () => number
  setTailSize: (tailSize: number) => void
  getBuffer: () => Buffer
  setBuffer: (buffer: Buffer) => void
  initConvolvers: () => void
}

export type VariableDelay = Transformer & {
  getMaxDelayTime: () => number
  setMaxDelayTime: (maxDelayTime: number) => void
  getDelayTime: () => Input
  getDecayTime: () => Input
  getReset: () => Input
  getFeedbackSource: () => Output
  getFeedbackSink: () => Input
  getFeedbackProcessor: () => Node
}

// ========== Dynamics Nodes ==========

export type Clipper = Transformer & {
  getMode: () => ClipperMode
  setMode: (mode: ClipperMode) => void
  getMinimum: () => Input
  getMaximum: () => Input
}

export type CompressorGate = Transformer & {
  getControl: () => Input
  getThreshold: () => Input
  getSoftness: () => Input
  getCompressionRatio: () => Input
  getGateRatio: () => Input
  getAttack: () => Input
  getRelease: () => Input
  getGain: () => Input
  getGainResponse: (channel: number, input: number) => number
}

export type DryWet = Producer & {
  getDry: () => Input
  getWet: () => Input
  getMixAmount: () => Input
  getA: () => Input
  getB: () => Input
}

export type Envelope = Producer & {
  getAttackShape: () => Shape
  setAttackShape: (shape: Shape) => void
  getReleaseShape: () => Shape
  setReleaseShape: (shape: Shape) => void
  getAttack: () => Input
  getRelease: () => Input
  getGate: () => Input
  getReset: () => Input
  getCurrentTime: () => number
}

export type Lag = Transformer & {
  getLagTime: () => Input
}

export type Shaper = Transformer & {
  getMode: () => ShaperMode
  setMode: (mode: ShaperMode) => void
  getDrive: () => Input
  getOutputSample: (channel: number, input: number) => number
}

export type TableShaper = Transformer & {
  getTables: () => BufferVector
  getInputInterpolation: () => Interpolation
  setInputInterpolation: (interpolation: Interpolation) => void
  getPositionInterpolation: () => Interpolation
  setPositionInterpolation: (interpolation: Interpolation) => void
  getPosition: () => Input
}

// ========== External Nodes ==========

export type MidiInput = Producer & {
  getInputMessages: () => MidiBuffer
  getProcessFunction: () => unknown
  setProcessFunction: (fn: unknown) => void
  processNote: () => void
  processNoteOn: () => void
  processNoteOff: () => void
  processNotePressure: () => void
  processControl: () => void
  processControlValue: () => void
  processProgram: () => void
  processChannelPressure: () => void
  processPitchWheel: () => void
  processAllNotesOff: () => void
  processSongPositionInQuarterNotes: () => void
  processClock: () => void
  processStart: () => void
  processContinue: () => void
  processStop: () => void
}

export type MidiOutput = Consumer & {
  getOutputMessages: () => MidiBuffer
  getProcessFunction: () => unknown
  setProcessFunction: (fn: unknown) => void
  processNote: () => void
  processNoteOn: () => void
  processNoteOff: () => void
  processNotePressure: () => void
  processControl: () => void
  processControlValue: () => void
  processProgram: () => void
  processChannelPressure: () => void
  processPitchWheel: () => void
  processAllNotesOff: () => void
  processSongPositionInQuarterNotes: () => void
  processClock: () => void
  processStart: () => void
  processContinue: () => void
  processStop: () => void
}

// ========== Filter Nodes ==========

export type Biquad = Transformer & {
  getMode: () => BiquadMode
  setMode: (mode: BiquadMode) => void
  getFrequency: () => Input
  getResonance: () => Input
  getAmplitude: () => Input
  getFrequencyResponse: (frequency: number) => number
}

export type Crossover = Node & {
  getInput: () => Input
  getFrequency: () => Input
  getLow: () => Output
  getHigh: () => Output
}

export type OnePole = Transformer & {
  getMode: () => OnePoleMode
  setMode: (mode: OnePoleMode) => void
  getFrequency: () => Input
}

// ========== Generator Nodes ==========

export type Noise = Producer & {
  getMode: () => NoiseMode
  setMode: (mode: NoiseMode) => void
}

export type Phasor = Producer & {
  getMode: () => PhasorMode
  setMode: (mode: PhasorMode) => void
  getFrequency: () => Input
  getReset: () => Input
}

export type SamplePlayer = Producer & {
  getSamples: () => BufferVector
  getInterpolation: () => Interpolation
  setInterpolation: (interpolation: Interpolation) => void
  getSpeed: () => Input
  getStartTime: () => Input
  getSampleIndex: () => Input
  getGate: () => Input
  getReset: () => Input
  getCurrentTime: () => number
}

export type TableOscillator = Producer & {
  getTables: () => BufferVector
  getPhaseInterpolation: () => Interpolation
  setPhaseInterpolation: (interpolation: Interpolation) => void
  getPositionInterpolation: () => Interpolation
  setPositionInterpolation: (interpolation: Interpolation) => void
  getPhase: () => Input
  getPosition: () => Input
}

// ========== Math Nodes ==========

export type AbsoluteValue = Transformer

export type BooleanMask = Transformer & {
  getMask: () => Input
}

export type Comparison = Transformer & {
  getMode: () => ComparisonMode
  setMode: (mode: ComparisonMode) => void
  getThreshold: () => Input
}

export type Division = Transformer & {
  getDivisor: () => Input
}

export type Floor = Transformer & {
  getDivisor: () => Input
}

export type ForwardFFT = Consumer & {
  getMagnitude: () => Output
  getPhase: () => Output
}

export type FrequencyToNote = Transformer & {
  getTuningFrequency: () => Input
}

export type Function = Transformer & {
  getFunction: () => (a: number, b: number) => number
  setFunction: (f: (a: number, b: number) => number) => void
  getA: () => Input
  getB: () => Input
}

export type Hyperbolic = Transformer & {
  getMode: () => HyperbolicMode
  setMode: (mode: HyperbolicMode) => void
}

export type Identity = Transformer

export type InverseFFT = Producer & {
  getMagnitude: () => Input
  getPhase: () => Input
}

export type Logarithm = Transformer & {
  getBase: () => Input
}

export type Modulo = Transformer & {
  getDivisor: () => Input
}

export type Multiplication = Transformer & {
  getFactor: () => Input
}

export type Negative = Transformer

export type NoteToFrequency = Transformer & {
  getTuningFrequency: () => Input
}

export type NotGate = Transformer

export type Power = Transformer & {
  getExponent: () => Input
}

export type Reciprocal = Transformer

export type Trigonometric = Transformer & {
  getMode: () => TrigonometricMode
  setMode: (mode: TrigonometricMode) => void
}

// ========== Trigger Nodes ==========

export type ClockTrigger = Producer & {
  getInterval: () => Input
  getDelayTime: () => Input
  getReset: () => Input
  getCurrentTime: () => number
}

export type Differentiator = Transformer & {
  getGate: () => Output
  getReset: () => Input
}

export type Integrator = Transformer & {
  getGate: () => Input
  getReset: () => Input
}

export type OnOff = Producer & {
  getOnTrigger: () => Input
  getOffTrigger: () => Input
}

export type ResetTrigger = Producer & {
  reset: () => void
  resetChannel: (channel: number) => void
}

export type SampleAndHold = Transformer & {
  getGate: () => Input
  getReset: () => Input
}

export type Sequencer = Producer & {
  getSequences: () => BufferVector
  getSequenceIndex: () => Input
  getPositionIndex: () => Input
}

export type TriggerHold = Transformer & {
  getHoldTime: () => Input
  getCurrentTime: () => number
}

// ========== Variable Nodes ==========

export type BufferDuration = Producer

export type BufferRate = Producer

export type SampleDuration = Producer

export type SampleRate = Producer

// ========== Utility Classes ==========

export type RealImaginary = {
  real: number[]
  imaginary: number[]
}

export type MagnitudePhase = {
  magnitude: number[]
  phase: number[]
}

export type NormalizedFFT = Deletable & {
  setup: (size: number) => void
  getSize: () => number
  getComplexSize: () => number
  toRealImaginary: (time: number[]) => RealImaginary
  fromRealImaginary: (real: number[], imaginary: number[]) => number[]
  toMagnitudePhase: (time: number[]) => MagnitudePhase
  fromMagnitudePhase: (magnitude: number[], phase: number[]) => number[]
}
