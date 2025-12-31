import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'

export class WrapperProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getNumChannels = (): Chainable<number> => {
    return chainable(this.call('getNumChannels', []))
  }
  getNumSamples = (): Chainable<number> => {
    return chainable(this.call('getNumSamples', []))
  }
  getChannelData = (_channel: number): never => {
    throw new Error('getChannelData is not implemented')
  }
  getSingleChannel = (channel: number): Chainable<WrapperProxy> => {
    return chainable(
      this.call<Target>('getSingleChannel', [channel]).then(
        (target) => new WrapperProxy(this.context, target),
      ),
    )
  }
  getSampleRange = (
    sampleOffset: number,
    numSamples: number,
  ): Chainable<WrapperProxy> => {
    return chainable(
      this.call<Target>('getSampleRange', [sampleOffset, numSamples]).then(
        (target) => new WrapperProxy(this.context, target),
      ),
    )
  }
  clear = (): Chainable<this> => {
    return chainable(this.call<void>('clear', []).then(() => this))
  }
  fill = (value: number): Chainable<this> => {
    return chainable(this.call<void>('fill', [value]).then(() => this))
  }
  apply = (_f: (x: number) => number): never => {
    throw new Error('apply is not implemented')
  }
  replaceWithApplicationOf = (
    _f: (x: number) => number,
    _src: WrapperProxy,
  ): never => {
    throw new Error('replaceWithApplicationOf is not implemented')
  }
  replaceWithApplicationOfTwoArgs = (
    _f: (x: number, y: number) => number,
    _src1: WrapperProxy,
    _src2: WrapperProxy,
  ): never => {
    throw new Error('replaceWithApplicationOfTwoArgs is not implemented')
  }
  copyFrom = (src: WrapperProxy): Chainable<this> => {
    return chainable(this.call<void>('copyFrom', [src]).then(() => this))
  }
  add = (value: number): Chainable<this> => {
    return chainable(this.call<void>('add', [value]).then(() => this))
  }
  addWrapper = (src: WrapperProxy): Chainable<this> => {
    return chainable(this.call<void>('addWrapper', [src]).then(() => this))
  }
  multiplyBy = (value: number): Chainable<this> => {
    return chainable(this.call<void>('multiplyBy', [value]).then(() => this))
  }
  multiplyByWrapper = (src: WrapperProxy): Chainable<this> => {
    return chainable(
      this.call<void>('multiplyByWrapper', [src]).then(() => this),
    )
  }
  addProductOf = (src: WrapperProxy, value: number): Chainable<this> => {
    return chainable(
      this.call<void>('addProductOf', [src, value]).then(() => this),
    )
  }
  addProductOfWrappers = (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Chainable<this> => {
    return chainable(
      this.call<void>('addProductOfWrappers', [src1, src2]).then(() => this),
    )
  }
  replaceWithNegativeOf = (src: WrapperProxy): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithNegativeOf', [src]).then(() => this),
    )
  }
  replaceWithAbsoluteValueOf = (src: WrapperProxy): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithAbsoluteValueOf', [src]).then(() => this),
    )
  }
  replaceWithSumOf = (src: WrapperProxy, value: number): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithSumOf', [src, value]).then(() => this),
    )
  }
  replaceWithSumOfWrappers = (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithSumOfWrappers', [src1, src2]).then(
        () => this,
      ),
    )
  }
  replaceWithProductOf = (
    src: WrapperProxy,
    value: number,
  ): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithProductOf', [src, value]).then(() => this),
    )
  }
  replaceWithProductOfWrappers = (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithProductOfWrappers', [src1, src2]).then(
        () => this,
      ),
    )
  }
  replaceWithMinOf = (src: WrapperProxy, value: number): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithMinOf', [src, value]).then(() => this),
    )
  }
  replaceWithMinOfWrappers = (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithMinOfWrappers', [src1, src2]).then(
        () => this,
      ),
    )
  }
  replaceWithMaxOf = (src: WrapperProxy, value: number): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithMaxOf', [src, value]).then(() => this),
    )
  }
  replaceWithMaxOfWrappers = (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Chainable<this> => {
    return chainable(
      this.call<void>('replaceWithMaxOfWrappers', [src1, src2]).then(
        () => this,
      ),
    )
  }
  getSample = (channel: number, sampleOffset: number): Chainable<number> => {
    return chainable(this.call('getSample', [channel, sampleOffset]))
  }
  setSample = (
    channel: number,
    sampleOffset: number,
    value: number,
  ): Chainable<void> => {
    return chainable(this.call('setSample', [channel, sampleOffset, value]))
  }
}
