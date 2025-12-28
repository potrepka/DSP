import type { Space, Type } from '../enums'

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
  Multiplication: new (type: Type, space: Space) => Node
}

export type NodeType = keyof NodeConstructorMap

type NodePropsMap = {
  Phasor: null
  Multiplication: {
    type: Type
    space: Space
  }
}

export type NodeProps<T extends NodeType> = NodePropsMap[T]

export type IncomingMessage<T extends NodeType> =
  | {
      messageType: 'createBuffer'
      id: string
      type?: Type
      space?: Space
      range?: number
      defaultValue?: number
      numChannels: number
      numSamples: number
    }
  | { messageType: 'destroyBuffer'; id: string }
  | { messageType: 'createNode'; id: string; nodeType: T; props?: NodeProps<T> }
  | { messageType: 'destroyNode'; id: string }
  | {
      messageType: 'setInputValue'
      nodeId: string
      inputName: string
      value: number
    }
  | {
      messageType: 'setInputChannelValue'
      nodeId: string
      inputName: string
      channel: number
      value: number
    }
  | {
      messageType: 'connect'
      sourceNodeId: string
      sourceOutputName: string
      destinationNodeId: string
      destinationInputName: string
    }
  | {
      messageType: 'disconnect'
      sourceNodeId: string
      sourceOutputName: string
      destinationNodeId: string
      destinationInputName: string
    }

export type OutgoingMessage = {
  messageType: 'setState'
  state: 'running' | 'closed'
}

export type NodeProcessor = {
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
  getInputMessages: () => MidiBuffer
  getOutputMessages: () => MidiBuffer
  process: (audioBuffer: Data, midibuffer: MidiBuffer) => void
  delete: () => void
}

export type Data = {
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
  delete: () => void
}

export type Wrapper = {
  getNumChannels: () => number
  getNumSamples: () => number
  getChannelData: (channel: number) => Float64Array
  clear: () => void
  fill: (value: number) => void
  getSample: (channel: number, sample: number) => number
  setSample: (channel: number, sample: number, value: number) => void
}

export type Buffer = {
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
  getPeak: () => number
  getRMS: () => number
  getData: () => Data
  getWrapper: () => Wrapper
  delete: () => void
}

export type Input = Buffer & {
  getConnections: () => number
  connect: (output: Output) => void
  disconnect: (output: Output) => void
  disconnectAll: () => void
}

export type Output = Buffer & {
  getConnections: () => number
  connect: (input: Input) => void
  disconnect: (input: Input) => void
  disconnectAll: () => void
}

export type Node = {
  isActive: () => boolean
  setActive: (active: boolean) => void
  getNumChannels: () => number
  setNumChannels: (numChannels: number) => void
  getNumSamples: () => number
  setNumSamples: (numSamples: number) => void
  getSampleRate: () => number
  setSampleRate: (sampleRate: number) => void
  addChild: (child: Node) => void
  removeChild: (child: Node) => void
  process: () => void
  delete: () => void
}

export type MidiBuffer = {
  clear: () => void
  delete: () => void
}

export type Phasor = Node & {
  getFrequency: () => Input
  getMode: () => Input
  getReset: () => Input
  getOutput: () => Output
}
