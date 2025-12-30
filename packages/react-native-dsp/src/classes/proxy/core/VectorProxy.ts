import type { ProxyContext, Target } from '../../../types'

export class VectorProxy<T extends { toTarget: () => Target }> {
  constructor(
    private readonly context: ProxyContext,
    private readonly target: Target,
    private readonly createItem: (target: Target) => T,
  ) {}
  get = async (index: number): Promise<T> => {
    const itemTarget = await this.context.call<Target>(this.target, 'get', [
      index,
    ])
    return this.createItem(itemTarget)
  }
  push_back = (value: T): Promise<void> => {
    return this.context.call(this.target, 'push_back', [value.toTarget()])
  }
  resize = (count: number, value: T): Promise<void> => {
    return this.context.call(this.target, 'resize', [count, value.toTarget()])
  }
  set = (index: number, value: T): Promise<boolean> => {
    return this.context.call(this.target, 'set', [index, value.toTarget()])
  }
  size = (): Promise<number> => {
    return this.context.call(this.target, 'size', [])
  }
}
