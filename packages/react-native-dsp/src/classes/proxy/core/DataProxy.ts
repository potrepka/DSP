import type { ProxyContext, Target } from '../../../types'
import { Proxy } from './Proxy'

export class DataProxy extends Proxy {
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
}
