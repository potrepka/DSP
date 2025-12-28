import type { InputMode, Space, Type } from '../enums'

export type Module = NodeConstructorMap & {
  NodeProcessor: new (
    numInputChannels: number,
    numOutputChannels: number,
    numSamples: number,
    sampleRate: number,
  ) => NodeProcessor
  Data: new (numChannels: number, numSamples: number) => Data
  Buffer: new (
    type: Type,
    space: Space,
    range: number,
    defaultValue: number,
    numChannels: number,
    numSamples: number,
  ) => Buffer
  MidiBuffer: new () => MidiBuffer
}

export type NodeConstructorMap = {
  Phasor: new () => Phasor
  Multiplication: new (type: Type, space: Space) => Multiplication
  Negative: new (type: Type, space: Space) => Negative
}

export type NodeType = keyof NodeConstructorMap

type NodePropsMap = {
  Phasor: never
  Multiplication: {
    type?: Type
    space?: Space
  }
  Negative: {
    type?: Type
    space?: Space
  }
}

export type NodeProps<T extends NodeType> = NodePropsMap[T]

export type IncomingMessage<T extends NodeType> =
  | {
      message: 'createBuffer'
      id: string
      type?: Type
      space?: Space
      range?: number
      defaultValue?: number
      numChannels: number
      numSamples: number
    }
  | { message: 'destroyBuffer'; id: string }
  | { message: 'createNode'; id: string; nodeType: T; props?: NodeProps<T> }
  | { message: 'destroyNode'; id: string }
  | {
      message: 'setInputValue'
      nodeId: string
      inputName: string
      value: number
    }
  | {
      message: 'setInputChannelValue'
      nodeId: string
      inputName: string
      channel: number
      value: number
    }
  | {
      message: 'connect'
      sourceNodeId: string
      sourceOutputName: string
      destinationNodeId: string
      destinationInputName: string
    }
  | {
      message: 'disconnect'
      sourceNodeId: string
      sourceOutputName: string
      destinationNodeId: string
      destinationInputName: string
    }

export type OutgoingMessage = {
  message: 'setState'
  state: 'running' | 'closed'
}

export type ModuleObject = {
  delete: () => void
}

export type Data = ModuleObject & {
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

export type Wrapper = ModuleObject & {
  getNumChannels: () => number
  getNumSamples: () => number
  getChannelData: (channel: number) => Float64Array
  getSingleChannel: (channel: number) => Wrapper
  getSampleRange: () => Wrapper
  clear: () => void
  fill: (value: number) => void
  apply: (f: (sample: number) => number) => void
  replaceWithApplicationOf: (
    f: (sample: number) => number,
    src: Wrapper,
  ) => void
  replaceWithApplicationOfTwoArgs: (
    f: (sample1: number, sample2: number) => number,
    src1: Wrapper,
    src2: Wrapper,
  ) => void
  replaceWithApplicationOfThreeArgs: (
    f: (sample1: number, sample2: number, sample3: number) => number,
    src1: Wrapper,
    src2: Wrapper,
    src3: Wrapper,
  ) => void
  copyFrom: (src: Wrapper) => void
  add: (value: number) => void
  addWrapper: (src: Wrapper) => void
  multiplyBy: (value: number) => void
  multiplyByWrapper: (src: Wrapper) => void
  addProductOf: (src: Wrapper, value: number) => void
  addProductOfWrappers: (src1: Wrapper, src2: Wrapper) => void
  replaceWithNegativeOf: (src: Wrapper) => void
  replaceWithAbsoluteValueOf: (src: Wrapper) => void
  replaceWithSumOf: (src: Wrapper, value: number) => void
  replaceWithSumOfWrappers: (src1: Wrapper, src2: Wrapper) => void
  replaceWithProductOf: (src: Wrapper, value: number) => void
  replaceWithProductOfWrappers: (src1: Wrapper, src2: Wrapper) => void
  replaceWithMinOf: (src: Wrapper, value: number) => void
  replaceWithMinOfWrappers: (src1: Wrapper, src2: Wrapper) => void
  replaceWithMaxOf: (src: Wrapper, value: number) => void
  replaceWithMaxOfWrappers: (src1: Wrapper, src2: Wrapper) => void
  getSample: (channel: number, sampleOffset: number) => number
  setSample: (channel: number, sampleOffset: number, value: number) => void
}

export type Buffer = ModuleObject & {
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
  getConnections: () => Output[]
  connect: (output: Output) => void
  disconnect: (output: Output) => void
  disconnectAll: () => void
  prepareNoLock: () => void
  processNoLock: () => void
}

export type Output = Buffer & {
  getConnections: () => Input[]
  connect: (input: Input) => void
  disconnect: (input: Input) => void
  disconnectAll: () => void
  prepareNoLock: () => void
  processNoLock: () => void
}

export type Lockable = ModuleObject & {
  lock: () => void
  unlock: () => void
}

export type Node = ModuleObject & {
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
  getInputs: () => Input[]
  getOutputs: () => Output[]
  getChildren: () => Node[]
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

export type NodeProcessor = ModuleObject & {
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
  getNodes: () => Node[]
  getInputMessages: () => MidiBuffer
  getOutputMessages: () => MidiBuffer
  process: (audioBuffer: Data, midibuffer: MidiBuffer) => void
}

export type MidiBuffer = ModuleObject & {
  addEvent: (midiMessage: MidiMessage, sample: number) => void
  addEvents: (
    midiBuffer: MidiBuffer,
    startSample: number,
    numSamples: number,
    sampleDeltaToAdd: number,
  ) => void
  clear: () => void
}

export type MidiMessage = ModuleObject & {
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

export type Phasor = Producer & {
  getFrequency: () => Input
  getMode: () => Input
  getReset: () => Input
}

export type Multiplication = Transformer & {
  getFactor: () => Input
}

export type Negative = Transformer
