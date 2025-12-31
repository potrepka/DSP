import type { ProxyContext, Target } from '../../../types'
import { BaseProxy } from './BaseProxy'

export class WrapperProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getNumChannels = (): Promise<number> => {
    return this.call('getNumChannels', [])
  }

  getNumSamples = (): Promise<number> => {
    return this.call('getNumSamples', [])
  }

  getChannelData = (_channel: number): never => {
    throw new Error('getChannelData is not implemented')
  }

  getSingleChannel = async (channel: number): Promise<WrapperProxy> => {
    const target = await this.call<Target>('getSingleChannel', [channel])
    return new WrapperProxy(this.context, target)
  }

  getSampleRange = async (
    sampleOffset: number,
    numSamples: number,
  ): Promise<WrapperProxy> => {
    const target = await this.call<Target>('getSampleRange', [
      sampleOffset,
      numSamples,
    ])
    return new WrapperProxy(this.context, target)
  }

  clear = async (): Promise<WrapperProxy> => {
    await this.call<void>('clear', [])
    return this
  }

  fill = async (value: number): Promise<WrapperProxy> => {
    await this.call<void>('fill', [value])
    return this
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

  copyFrom = async (src: WrapperProxy): Promise<WrapperProxy> => {
    await this.call<void>('copyFrom', [src])
    return this
  }

  add = async (value: number): Promise<WrapperProxy> => {
    await this.call<void>('add', [value])
    return this
  }

  addWrapper = async (src: WrapperProxy): Promise<WrapperProxy> => {
    await this.call<void>('addWrapper', [src])
    return this
  }

  multiplyBy = async (value: number): Promise<WrapperProxy> => {
    await this.call<void>('multiplyBy', [value])
    return this
  }

  multiplyByWrapper = async (src: WrapperProxy): Promise<WrapperProxy> => {
    await this.call<void>('multiplyByWrapper', [src])
    return this
  }

  addProductOf = async (
    src: WrapperProxy,
    value: number,
  ): Promise<WrapperProxy> => {
    await this.call<void>('addProductOf', [src, value])
    return this
  }

  addProductOfWrappers = async (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Promise<WrapperProxy> => {
    await this.call<void>('addProductOfWrappers', [src1, src2])
    return this
  }

  replaceWithNegativeOf = async (src: WrapperProxy): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithNegativeOf', [src])
    return this
  }

  replaceWithAbsoluteValueOf = async (
    src: WrapperProxy,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithAbsoluteValueOf', [src])
    return this
  }

  replaceWithSumOf = async (
    src: WrapperProxy,
    value: number,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithSumOf', [src, value])
    return this
  }

  replaceWithSumOfWrappers = async (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithSumOfWrappers', [src1, src2])
    return this
  }

  replaceWithProductOf = async (
    src: WrapperProxy,
    value: number,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithProductOf', [src, value])
    return this
  }

  replaceWithProductOfWrappers = async (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithProductOfWrappers', [src1, src2])
    return this
  }

  replaceWithMinOf = async (
    src: WrapperProxy,
    value: number,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithMinOf', [src, value])
    return this
  }

  replaceWithMinOfWrappers = async (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithMinOfWrappers', [src1, src2])
    return this
  }

  replaceWithMaxOf = async (
    src: WrapperProxy,
    value: number,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithMaxOf', [src, value])
    return this
  }

  replaceWithMaxOfWrappers = async (
    src1: WrapperProxy,
    src2: WrapperProxy,
  ): Promise<WrapperProxy> => {
    await this.call<void>('replaceWithMaxOfWrappers', [src1, src2])
    return this
  }

  getSample = (channel: number, sampleOffset: number): Promise<number> => {
    return this.call('getSample', [channel, sampleOffset])
  }

  setSample = (
    channel: number,
    sampleOffset: number,
    value: number,
  ): Promise<void> => {
    return this.call('setSample', [channel, sampleOffset, value])
  }
}
