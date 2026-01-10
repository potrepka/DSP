import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'

export class DataProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getNumChannels = (): Chainable<number> => {
    return chainable(this.call('getNumChannels', []))
  }
  getNumSamples = (): Chainable<number> => {
    return chainable(this.call('getNumSamples', []))
  }
  setSize = (numChannels: number, numSamples: number): Chainable<void> => {
    return chainable(this.call('setSize', [numChannels, numSamples]))
  }
  clear = (): Chainable<void> => {
    return chainable(this.call('clear', []))
  }
  getReadChannelData = (channel: number): Chainable<number[]> => {
    return chainable(this.call('getReadChannelData', [channel]))
  }
  getWriteChannelData = (_channel: number): never => {
    throw new Error('getWriteChannelData is not implemented')
  }
  getMagnitude = (
    channel: number,
    startSample: number,
    numSamples: number,
  ): Chainable<number> => {
    return chainable(
      this.call('getMagnitude', [channel, startSample, numSamples]),
    )
  }
  getRMSLevel = (
    channel: number,
    startSample: number,
    numSamples: number,
  ): Chainable<number> => {
    return chainable(
      this.call('getRMSLevel', [channel, startSample, numSamples]),
    )
  }
  getReadData = (): Chainable<number[][]> => {
    return chainable(this.call('getReadData', []))
  }
  getWriteData = (): never => {
    throw new Error('getWriteData is not implemented')
  }
}
