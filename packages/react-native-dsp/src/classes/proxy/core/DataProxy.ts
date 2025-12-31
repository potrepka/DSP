import type { ProxyContext, Target } from '../../../types'
import { BaseProxy } from './BaseProxy'

export class DataProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getNumChannels = (): Promise<number> => {
    return this.call('getNumChannels', [])
  }

  getNumSamples = (): Promise<number> => {
    return this.call('getNumSamples', [])
  }

  setSize = (numChannels: number, numSamples: number): Promise<void> => {
    return this.call('setSize', [numChannels, numSamples])
  }

  clear = (): Promise<void> => {
    return this.call('clear', [])
  }

  getReadChannelData = (_channel: number): never => {
    throw new Error('getReadChannelData is not implemented')
  }

  getWriteChannelData = (_channel: number): never => {
    throw new Error('getWriteChannelData is not implemented')
  }

  getMagnitude = (
    channel: number,
    startSample: number,
    numSamples: number,
  ): Promise<number> => {
    return this.call('getMagnitude', [channel, startSample, numSamples])
  }

  getRMSLevel = (
    channel: number,
    startSample: number,
    numSamples: number,
  ): Promise<number> => {
    return this.call('getRMSLevel', [channel, startSample, numSamples])
  }

  getReadData = (): never => {
    throw new Error('getReadData is not implemented')
  }

  getWriteData = (): never => {
    throw new Error('getWriteData is not implemented')
  }
}
