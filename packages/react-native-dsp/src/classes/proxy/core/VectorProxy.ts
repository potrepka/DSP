import type { ProxyContext, Target } from '../../../types'
import { BaseProxy } from './BaseProxy'

export class VectorProxy<T extends BaseProxy> extends BaseProxy {
  constructor(
    context: ProxyContext,
    target: Target,
    private readonly createItem: (target: Target) => T,
  ) {
    super(context, target)
  }

  get = async (index: number): Promise<T> => {
    const itemTarget = await this.call<Target>('get', [index])
    return this.createItem(itemTarget)
  }

  push_back = (value: T): Promise<void> => {
    return this.call('push_back', [value])
  }

  resize = (count: number, value: T): Promise<void> => {
    return this.call('resize', [count, value])
  }

  set = (index: number, value: T): Promise<boolean> => {
    return this.call('set', [index, value])
  }

  size = (): Promise<number> => {
    return this.call('size', [])
  }
}
