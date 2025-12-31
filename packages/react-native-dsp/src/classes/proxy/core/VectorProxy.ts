import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'

export class VectorProxy<T extends BaseProxy> extends BaseProxy {
  constructor(
    context: ProxyContext,
    target: Target,
    private readonly createItem: (target: Target) => T,
  ) {
    super(context, target)
  }
  get = (index: number): Chainable<T> => {
    return chainable(
      this.call<Target>('get', [index]).then((itemTarget) =>
        this.createItem(itemTarget),
      ),
    )
  }
  push_back = (value: T): Chainable<void> => {
    return chainable(this.call('push_back', [value]))
  }
  resize = (count: number, value: T): Chainable<void> => {
    return chainable(this.call('resize', [count, value]))
  }
  set = (index: number, value: T): Chainable<boolean> => {
    return chainable(this.call('set', [index, value]))
  }
  size = (): Chainable<number> => {
    return chainable(this.call('size', []))
  }
}
