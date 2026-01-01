import { InputMode, Space, Type } from '../enums/global'
import { RecorderMode } from '../enums/node'

export type AudioModule = GlobalFunctionMap & ObjectConstructorMap

export type GlobalFunctionMap = {
  byteToUnipolar: (value: number) => number
  unipolarToByte: (value: number) => number
  shortToUnipolar: (value: number) => number
  unipolarToShort: (value: number) => number
  clip: (value: number, min: number, max: number) => number
  wrap: (value: number, max: number) => number
  linear: (
    data: Float64Array,
    size: number,
    index: number,
    defaultValue?: number,
  ) => number
  hermite: (
    data: Float64Array,
    size: number,
    index: number,
    defaultValue?: number,
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
    space: Space,
    range: number,
    defaultValue: number,
    numChannels: number,
    numSamples: number,
  ) => Buffer
  Input: new (
    type: Type,
    space: Space,
    range: number,
    defaultValue: number,
    numChannels: number,
    numSamples: number,
  ) => Input
  Output: new (
    type: Type,
    space: Space,
    range: number,
    defaultValue: number,
    numChannels: number,
    numSamples: number,
  ) => Output
  Lockable: new () => Lockable
  Engine: new () => Engine
  Node: new () => Node
  Consumer: new (type: Type, space: Space) => Consumer
  Producer: new (type: Type, space: Space) => Producer
  Transformer: {
    new (type: Type, space: Space): Transformer
    new (inputType: Type, outputType: Type, space: Space): Transformer
    new (
      inputType: Type,
      outputType: Type,
      inputSpace: Space,
      outputSpace: Space,
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
  Recorder: new (type: Type, space: Space, defaultValue: number) => Recorder

  // Channel Nodes
  ChannelMerger: new (type: Type, space: Space) => ChannelMerger
  ChannelSplitter: new (type: Type, space: Space) => ChannelSplitter
  MidSide: new (type: Type, space: Space) => MidSide
  Spread: new (type: Type, space: Space) => Spread
  StereoPanner: new (type: Type, space: Space) => StereoPanner

  // Delay Nodes
  Convolver: new () => Convolver
  VariableDelay: new (type: Type) => VariableDelay

  // Dynamics Nodes
  Clipper: new (type: Type, space: Space) => Clipper
  CompressorGate: new () => CompressorGate
  DryWet: new (type: Type, space: Space) => DryWet
  Envelope: new () => Envelope
  Lag: new (type: Type) => Lag
  Shaper: new (space: Space) => Shaper

  // External Nodes
  MidiInput: new (midiBuffer: MidiBuffer, type: Type) => MidiInput
  MidiOutput: new (midiBuffer: MidiBuffer, type: Type) => MidiOutput

  // Filter Nodes
  Biquad: new () => Biquad
  Crossover: new () => Crossover
  OnePole: new (type: Type) => OnePole

  // Generator Nodes
  FunctionOscillator: new (type: Type) => FunctionOscillator
  Noise: new () => Noise
  Phasor: new () => Phasor
  SamplePlayer: new (type: Type) => SamplePlayer
  TableOscillator: new (type: Type) => TableOscillator

  // Math Nodes
  AbsoluteValue: new (type: Type, space: Space) => AbsoluteValue
  BooleanMask: new (type: Type, space: Space) => BooleanMask
  Comparison: new (type: Type, space: Space) => Comparison
  Division: new (type: Type, space: Space) => Division
  Floor: new (type: Type, space: Space) => Floor
  ForwardFFT: new () => ForwardFFT
  FrequencyToNote: new (space: Space) => FrequencyToNote
  Hyperbolic: new (space: Space) => Hyperbolic
  Identity: {
    new (type: Type, space: Space): Identity
    new (inputType: Type, outputType: Type, space: Space): Identity
    new (
      inputType: Type,
      outputType: Type,
      inputSpace: Space,
      outputSpace: Space,
    ): Identity
  }
  InverseFFT: new () => InverseFFT
  Logarithm: new (space: Space) => Logarithm
  Modulo: new (type: Type, space: Space) => Modulo
  Multiplication: new (type: Type, space: Space) => Multiplication
  Negative: new (type: Type, space: Space) => Negative
  NoteToFrequency: new (space: Space) => NoteToFrequency
  NotGate: new (space: Space) => NotGate
  Power: new (space: Space) => Power
  Reciprocal: new (type: Type, space: Space) => Reciprocal
  Trigonometric: new (space: Space) => Trigonometric

  // Trigger Nodes
  ClockTrigger: new () => ClockTrigger
  Differentiator: new (type: Type) => Differentiator
  Integrator: new (type: Type) => Integrator
  OnOff: new () => OnOff
  ResetTrigger: new () => ResetTrigger
  SampleAndHold: new (type: Type) => SampleAndHold
  Sequencer: new (type: Type, space: Space) => Sequencer
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
    space: Space
    range: number
    defaultValue: number
    numChannels: number
    numSamples: number
  }
  Input: {
    type: Type
    space: Space
    range: number
    defaultValue: number
    numChannels: number
    numSamples: number
  }
  Output: {
    type: Type
    space: Space
    range: number
    defaultValue: number
    numChannels: number
    numSamples: number
  }
  Lockable: unknown
  Engine: unknown
  Node: unknown
  Consumer: { type: Type; space: Space }
  Producer: { type: Type; space: Space }
  Transformer: {
    type?: Type
    space?: Space
    inputType?: Type
    outputType?: Type
    inputSpace?: Space
    outputSpace?: Space
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
    space: Space
    defaultValue: number
    numChannels: number
  }

  // Nodes - Channel
  ChannelMerger: { type: Type; space: Space }
  ChannelSplitter: { type: Type; space: Space }
  MidSide: { type: Type; space: Space }
  Spread: { type: Type; space: Space }
  StereoPanner: { type: Type; space: Space }

  // Nodes - Delay
  Convolver: unknown
  VariableDelay: { type: Type }

  // Nodes - Dynamics
  Clipper: { type: Type; space: Space }
  CompressorGate: unknown
  DryWet: { type: Type; space: Space }
  Envelope: unknown
  Lag: { type: Type }
  Shaper: { space: Space }

  // Nodes - External
  MidiInput: { midiBuffer: MidiBuffer; type: Type }
  MidiOutput: { midiBuffer: MidiBuffer; type: Type }

  // Nodes - Filter
  Biquad: unknown
  Crossover: unknown
  OnePole: { type: Type }

  // Nodes - Generator
  FunctionOscillator: { type: Type }
  Noise: unknown
  Phasor: unknown
  SamplePlayer: { type: Type }
  TableOscillator: { type: Type }

  // Nodes - Math
  AbsoluteValue: { type: Type; space: Space }
  BooleanMask: { type: Type; space: Space }
  Comparison: { type: Type; space: Space }
  Division: { type: Type; space: Space }
  Floor: { type: Type; space: Space }
  ForwardFFT: unknown
  FrequencyToNote: { space: Space }
  Hyperbolic: { space: Space }
  Identity: {
    type?: Type
    space?: Space
    inputType?: Type
    outputType?: Type
    inputSpace?: Space
    outputSpace?: Space
    numChannels: number
  }
  InverseFFT: unknown
  Logarithm: { space: Space }
  Modulo: { type: Type; space: Space }
  Multiplication: { type: Type; space: Space }
  Negative: { type: Type; space: Space }
  NoteToFrequency: { space: Space }
  NotGate: { space: Space }
  Power: { space: Space }
  Reciprocal: { type: Type; space: Space }
  Trigonometric: { space: Space }

  // Nodes - Trigger
  ClockTrigger: unknown
  Differentiator: { type: Type }
  Integrator: { type: Type }
  OnOff: unknown
  ResetTrigger: unknown
  SampleAndHold: { type: Type }
  Sequencer: { type: Type; space: Space }
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
  getMagnitude: () => number
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
  getSpace: () => Space
  setSpace: (space: Space) => void
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
  getSpread: () => Input
  getMode: () => Input
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
  getMin: () => Input
  getMax: () => Input
  getMode: () => Input
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
  getAttack: () => Input
  getRelease: () => Input
  getAttackShape: () => Input
  getReleaseShape: () => Input
  getGate: () => Input
  getReset: () => Input
  getCurrentTime: () => number
}

export type Lag = Transformer & {
  getLagTime: () => Input
}

export type Shaper = Transformer & {
  getDrive: () => Input
  getMode: () => Input
  getOutputSignal: (channel: number, input: number) => number
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
  getFrequency: () => Input
  getResonance: () => Input
  getAmplitude: () => Input
  getMode: () => Input
  getFrequencyResponse: (frequency: number) => number
}

export type Crossover = Node & {
  getInput: () => Input
  getFrequency: () => Input
  getLow: () => Output
  getHigh: () => Output
}

export type OnePole = Transformer & {
  getFrequency: () => Input
  getMode: () => Input
}

// ========== Generator Nodes ==========

export type FunctionOscillator = Producer & {
  getFunction: () => unknown
  setFunction: (fn: (phase: number) => number) => void
  getPhase: () => Input
}

export type Noise = Producer & {
  getMode: () => Input
}

export type Phasor = Producer & {
  getFrequency: () => Input
  getMode: () => Input
  getReset: () => Input
}

export type SamplePlayer = Producer & {
  getSamples: () => BufferVector
  getSpeed: () => Input
  getStartTime: () => Input
  getSampleIndex: () => Input
  getInterpolation: () => Input
  getGate: () => Input
  getReset: () => Input
  getCurrentTime: () => number
}

export type TableOscillator = Producer & {
  getTables: () => BufferVector
  getPhase: () => Input
  getPosition: () => Input
  getPhaseInterpolation: () => Input
  getPositionInterpolation: () => Input
}

// ========== Math Nodes ==========

export type AbsoluteValue = Transformer

export type BooleanMask = Transformer & {
  getMask: () => Input
}

export type Comparison = Transformer & {
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

export type Hyperbolic = Transformer & {
  getMode: () => Input
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
  getMode: () => Input
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

export type NormalizedFFT = Deletable & {
  setup: (size: number) => void
  getSize: () => number
  getComplexSize: () => number
  toRealImaginary: (
    timeChannel: Float64Array,
    realChannel: Float64Array,
    imaginaryChannel: Float64Array,
  ) => void
  fromRealImaginary: (
    realChannel: Float64Array,
    imaginaryChannel: Float64Array,
    timeChannel: Float64Array,
  ) => void
  toMagnitudePhase: (
    timeChannel: Float64Array,
    magnitudeChannel: Float64Array,
    phaseChannel: Float64Array,
  ) => void
  fromMagnitudePhase: (
    magnitudeChannel: Float64Array,
    phaseChannel: Float64Array,
    timeChannel: Float64Array,
  ) => void
}
