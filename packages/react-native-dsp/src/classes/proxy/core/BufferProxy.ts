import { InputMode, Space, Type } from '../../../enums'
import type { ProxyContext, Target } from '../../../types'
import { BaseProxy } from './BaseProxy'
import { DataProxy } from './DataProxy'
import { VectorProxy } from './VectorProxy'
import { WrapperProxy } from './WrapperProxy'

export class BufferProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getType = (): Promise<Type> => {
    return this.call('getType', [])
  }

  setType = (type: Type): Promise<void> => {
    return this.call('setType', [type])
  }

  getSpace = (): Promise<Space> => {
    return this.call('getSpace', [])
  }

  setSpace = (space: Space): Promise<void> => {
    return this.call('setSpace', [space])
  }

  getRange = (): Promise<number> => {
    return this.call('getRange', [])
  }

  setRange = (range: number): Promise<void> => {
    return this.call('setRange', [range])
  }

  getDefaultValue = (): Promise<number> => {
    return this.call('getDefaultValue', [])
  }

  setDefaultValue = (value: number): Promise<void> => {
    return this.call('setDefaultValue', [value])
  }

  getNumChannels = (): Promise<number> => {
    return this.call('getNumChannels', [])
  }

  setNumChannels = (numChannels: number): Promise<void> => {
    return this.call('setNumChannels', [numChannels])
  }

  getNumSamples = (): Promise<number> => {
    return this.call('getNumSamples', [])
  }

  setNumSamples = (numSamples: number): Promise<void> => {
    return this.call('setNumSamples', [numSamples])
  }

  setSize = (numChannels: number, numSamples: number): Promise<void> => {
    return this.call('setSize', [numChannels, numSamples])
  }

  getChannelValues = async (): Promise<Float64Array> => {
    const result = await this.call<number[]>('getChannelValues', [])
    return new Float64Array(result)
  }

  setChannelValues = (values: Float64Array): Promise<void> => {
    return this.call('setChannelValues', [Array.from(values)])
  }

  getChannelValue = (channel: number): Promise<number> => {
    return this.call('getChannelValue', [channel])
  }

  setChannelValue = (channel: number, value: number): Promise<void> => {
    return this.call('setChannelValue', [channel, value])
  }

  setAllChannelValues = (value: number): Promise<void> => {
    return this.call('setAllChannelValues', [value])
  }

  getPeak = async (): Promise<Float64Array> => {
    const result = await this.call<number[]>('getPeak', [])
    return new Float64Array(result)
  }

  getRMS = async (): Promise<Float64Array> => {
    const result = await this.call<number[]>('getRMS', [])
    return new Float64Array(result)
  }

  getData = async (): Promise<DataProxy> => {
    const target = await this.call<Target>('getData', [])
    return new DataProxy(this.context, target)
  }

  getWrapper = async (): Promise<WrapperProxy> => {
    const target = await this.call<Target>('getWrapper', [])
    return new WrapperProxy(this.context, target)
  }
}

export class InputProxy extends BufferProxy {
  getMode = (): Promise<InputMode> => {
    return this.call('getMode', [])
  }

  setMode = (mode: InputMode): Promise<void> => {
    return this.call('setMode', [mode])
  }

  getConnections = async (): Promise<VectorProxy<OutputProxy>> => {
    const target = await this.call<Target>('getConnections', [])
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      return new OutputProxy(this.context, itemTarget)
    })
  }

  connect = (output: OutputProxy): Promise<void> => {
    return this.call('connect', [output])
  }

  disconnect = (output: OutputProxy): Promise<void> => {
    return this.call('disconnect', [output])
  }

  disconnectAll = (): Promise<void> => {
    return this.call('disconnectAll', [])
  }

  prepareNoLock = (): Promise<void> => {
    return this.call('prepareNoLock', [])
  }

  processNoLock = (): Promise<void> => {
    return this.call('processNoLock', [])
  }
}

export class OutputProxy extends BufferProxy {
  getConnections = async (): Promise<VectorProxy<InputProxy>> => {
    const target = await this.call<Target>('getConnections', [])
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      return new InputProxy(this.context, itemTarget)
    })
  }

  connect = (input: InputProxy): Promise<void> => {
    return this.call('connect', [input])
  }

  disconnect = (input: InputProxy): Promise<void> => {
    return this.call('disconnect', [input])
  }

  disconnectAll = (): Promise<void> => {
    return this.call('disconnectAll', [])
  }

  prepareNoLock = (): Promise<void> => {
    return this.call('prepareNoLock', [])
  }

  processNoLock = (): Promise<void> => {
    return this.call('processNoLock', [])
  }
}
