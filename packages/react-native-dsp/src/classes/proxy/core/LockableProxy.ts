import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'

export class LockableProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  lock = (): Chainable<void> => {
    return chainable(this.call('lock', []))
  }
  unlock = (): Chainable<void> => {
    return chainable(this.call('unlock', []))
  }
}
