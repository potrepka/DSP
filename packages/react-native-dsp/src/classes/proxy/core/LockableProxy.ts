import type { ProxyContext, Target } from '../../../types'
import { BaseProxy } from './BaseProxy'

export class LockableProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  lock = (): Promise<void> => {
    return this.call('lock', [])
  }

  unlock = (): Promise<void> => {
    return this.call('unlock', [])
  }
}
