import type { Space, Type } from '../enums'

export type Module = {
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
  getChannelValue: (channel: number) => number
  setSingleChannelValue: (channel: number, value: number) => void
  setAllChannelValues: (value: number) => void
  getPeak: () => number
  getRMS: () => number
  getData: () => Data
  getWrapper: () => Wrapper
  delete: () => void
}

export type Input = {
  connect: (output: Output) => void
  disconnect: (output: Output) => void
  disconnectAll: () => void
}

export type Output = {
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
}

export type MidiBuffer = {
  clear: () => void
  delete: () => void
}
