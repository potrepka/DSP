import { Domain, InputMode, Type } from '../../../enums/global'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'
import { DataProxy } from './DataProxy'
import { VectorProxy } from './VectorProxy'
import { WrapperProxy } from './WrapperProxy'

export class BufferProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getType = (): Chainable<Type> => {
    return chainable(this.call('getType', []))
  }
  setType = (type: Type): Chainable<void> => {
    return chainable(this.call('setType', [type]))
  }
  getDomain = (): Chainable<Domain> => {
    return chainable(this.call('getDomain', []))
  }
  setDomain = (domain: Domain): Chainable<void> => {
    return chainable(this.call('setDomain', [domain]))
  }
  getRange = (): Chainable<number> => {
    return chainable(this.call('getRange', []))
  }
  setRange = (range: number): Chainable<void> => {
    return chainable(this.call('setRange', [range]))
  }
  getDefaultValue = (): Chainable<number> => {
    return chainable(this.call('getDefaultValue', []))
  }
  setDefaultValue = (value: number): Chainable<void> => {
    return chainable(this.call('setDefaultValue', [value]))
  }
  getNumChannels = (): Chainable<number> => {
    return chainable(this.call('getNumChannels', []))
  }
  setNumChannels = (numChannels: number): Chainable<void> => {
    return chainable(this.call('setNumChannels', [numChannels]))
  }
  getNumSamples = (): Chainable<number> => {
    return chainable(this.call('getNumSamples', []))
  }
  setNumSamples = (numSamples: number): Chainable<void> => {
    return chainable(this.call('setNumSamples', [numSamples]))
  }
  setSize = (numChannels: number, numSamples: number): Chainable<void> => {
    return chainable(this.call('setSize', [numChannels, numSamples]))
  }
  getChannelValues = (): Chainable<Float64Array> => {
    return chainable(
      this.call<number[]>('getChannelValues', []).then(
        (result) => new Float64Array(result),
      ),
    )
  }
  setChannelValues = (values: Float64Array): Chainable<void> => {
    return chainable(this.call('setChannelValues', [Array.from(values)]))
  }
  getChannelValue = (channel: number): Chainable<number> => {
    return chainable(this.call('getChannelValue', [channel]))
  }
  setChannelValue = (channel: number, value: number): Chainable<void> => {
    return chainable(this.call('setChannelValue', [channel, value]))
  }
  setAllChannelValues = (value: number): Chainable<void> => {
    return chainable(this.call('setAllChannelValues', [value]))
  }
  getPeak = (): Chainable<Float64Array> => {
    return chainable(
      this.call<number[]>('getPeak', []).then(
        (result) => new Float64Array(result),
      ),
    )
  }
  getRMS = (): Chainable<Float64Array> => {
    return chainable(
      this.call<number[]>('getRMS', []).then(
        (result) => new Float64Array(result),
      ),
    )
  }
  getData = (): Chainable<DataProxy> => {
    return chainable(
      this.call<Target>('getData', []).then(
        (target) => new DataProxy(this.context, target),
      ),
    )
  }
  getWrapper = (): Chainable<WrapperProxy> => {
    return chainable(
      this.call<Target>('getWrapper', []).then(
        (target) => new WrapperProxy(this.context, target),
      ),
    )
  }
}

export class InputProxy extends BufferProxy {
  getMode = (): Chainable<InputMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: InputMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getConnections = (): Chainable<VectorProxy<OutputProxy>> => {
    return chainable(
      this.call<Target>('getConnections', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new OutputProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  connect = (output: OutputProxy): Chainable<void> => {
    return chainable(this.call('connect', [output]))
  }
  disconnect = (output: OutputProxy): Chainable<void> => {
    return chainable(this.call('disconnect', [output]))
  }
  disconnectAll = (): Chainable<void> => {
    return chainable(this.call('disconnectAll', []))
  }
  prepareNoLock = (): Chainable<void> => {
    return chainable(this.call('prepareNoLock', []))
  }
  processNoLock = (): Chainable<void> => {
    return chainable(this.call('processNoLock', []))
  }
}

export class OutputProxy extends BufferProxy {
  getConnections = (): Chainable<VectorProxy<InputProxy>> => {
    return chainable(
      this.call<Target>('getConnections', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new InputProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  connect = (input: InputProxy): Chainable<void> => {
    return chainable(this.call('connect', [input]))
  }
  disconnect = (input: InputProxy): Chainable<void> => {
    return chainable(this.call('disconnect', [input]))
  }
  disconnectAll = (): Chainable<void> => {
    return chainable(this.call('disconnectAll', []))
  }
  prepareNoLock = (): Chainable<void> => {
    return chainable(this.call('prepareNoLock', []))
  }
  processNoLock = (): Chainable<void> => {
    return chainable(this.call('processNoLock', []))
  }
}
