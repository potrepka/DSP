import type { Data, MidiBuffer, ProxyContext, Target } from '../../../types'
import { BaseProxy } from './BaseProxy'
import { InputProxy, OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'
import { VectorProxy } from './VectorProxy'

export class NodeProcessorProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  isActive = (): Promise<boolean> => {
    return this.call('isActive', [])
  }

  setActive = (active: boolean): Promise<void> => {
    return this.call('setActive', [active])
  }

  getAudioInput = async (): Promise<OutputProxy> => {
    const target = await this.call<Target>('getAudioInput', [])
    return new OutputProxy(this.context, target)
  }

  getAudioOutput = async (): Promise<InputProxy> => {
    const target = await this.call<Target>('getAudioOutput', [])
    return new InputProxy(this.context, target)
  }

  getAudioInputClipping = async (): Promise<OutputProxy> => {
    const target = await this.call<Target>('getAudioInputClipping', [])
    return new OutputProxy(this.context, target)
  }

  getAudioOutputClipping = async (): Promise<OutputProxy> => {
    const target = await this.call<Target>('getAudioOutputClipping', [])
    return new OutputProxy(this.context, target)
  }

  getNumInputChannels = (): Promise<number> => {
    return this.call('getNumInputChannels', [])
  }

  setNumInputChannels = (numChannels: number): Promise<void> => {
    return this.call('setNumInputChannels', [numChannels])
  }

  getNumOutputChannels = (): Promise<number> => {
    return this.call('getNumOutputChannels', [])
  }

  setNumOutputChannels = (numChannels: number): Promise<void> => {
    return this.call('setNumOutputChannels', [numChannels])
  }

  getNumSamples = (): Promise<number> => {
    return this.call('getNumSamples', [])
  }

  setNumSamples = (numSamples: number): Promise<void> => {
    return this.call('setNumSamples', [numSamples])
  }

  getSampleRate = (): Promise<number> => {
    return this.call('getSampleRate', [])
  }

  setSampleRate = (sampleRate: number): Promise<void> => {
    return this.call('setSampleRate', [sampleRate])
  }

  setInputSize = (numChannels: number, numSamples: number): Promise<void> => {
    return this.call('setInputSize', [numChannels, numSamples])
  }

  setOutputSize = (numChannels: number, numSamples: number): Promise<void> => {
    return this.call('setOutputSize', [numChannels, numSamples])
  }

  getDefaultNode = async (): Promise<NodeProxy> => {
    const target = await this.call<Target>('getDefaultNode', [])
    return new NodeProxy(this.context, target)
  }

  getNodes = async (): Promise<VectorProxy<NodeProxy>> => {
    const target = await this.call<Target>('getNodes', [])
    return new VectorProxy(this.context, target, (nodeTarget: Target) => {
      return new NodeProxy(this.context, nodeTarget)
    })
  }

  getInputMessages = (): MidiBuffer => {
    throw new Error(
      'getInputMessages returns a complex object that cannot be proxied',
    )
  }

  getOutputMessages = (): MidiBuffer => {
    throw new Error(
      'getOutputMessages returns a complex object that cannot be proxied',
    )
  }

  process = (_audioBuffer: Data, _midiBuffer: MidiBuffer): Promise<void> => {
    throw new Error(
      'process requires Data and MidiBuffer objects from the worklet',
    )
  }
}
