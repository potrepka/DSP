import type { ProxyContext, Target } from '../../../types'
import { InputProxy, OutputProxy } from './BufferProxy'
import { Proxy } from './Proxy'
import { VectorProxy } from './VectorProxy'

export class NodeProxy extends Proxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  protected createInput = async (inputName: string): Promise<InputProxy> => {
    const target = await this.call<Target>(`get${inputName}`, [])
    return new InputProxy(this.context, target)
  }

  protected createOutput = async (outputName: string): Promise<OutputProxy> => {
    const target = await this.call<Target>(`get${outputName}`, [])
    return new OutputProxy(this.context, target)
  }

  isActive = (): Promise<boolean> => {
    return this.call('isActive', [])
  }

  setActive = (active: boolean): Promise<void> => {
    return this.call('setActive', [active])
  }

  getNumChannels = (): Promise<number> => {
    return this.call('getNumChannels', [])
  }

  setNumChannels = (numChannels: number): Promise<void> => {
    return this.call('setNumChannels', [numChannels])
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

  getOneOverNumSamples = (): Promise<number> => {
    return this.call('getOneOverNumSamples', [])
  }

  getOneOverSampleRate = (): Promise<number> => {
    return this.call('getOneOverSampleRate', [])
  }

  getInputs = async (): Promise<VectorProxy<InputProxy>> => {
    const target = await this.call<Target>('getInputs', [])
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      return new InputProxy(this.context, itemTarget)
    })
  }

  getOutputs = async (): Promise<VectorProxy<OutputProxy>> => {
    const target = await this.call<Target>('getOutputs', [])
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      return new OutputProxy(this.context, itemTarget)
    })
  }

  getChildren = async (): Promise<VectorProxy<NodeProxy>> => {
    const target = await this.call<Target>('getChildren', [])
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      return new NodeProxy(this.context, itemTarget)
    })
  }

  addChild = (child: NodeProxy): Promise<void> => {
    return this.call('addChild', [child])
  }

  removeChild = (child: NodeProxy): Promise<void> => {
    return this.call('removeChild', [child])
  }

  sortChildren = (): Promise<void> => {
    return this.call('sortChildren', [])
  }

  disconnectAll = (): Promise<void> => {
    return this.call('disconnectAll', [])
  }

  process = (): Promise<void> => {
    return this.call('process', [])
  }
}
