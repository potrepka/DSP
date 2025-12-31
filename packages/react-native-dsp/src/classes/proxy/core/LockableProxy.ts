import type { ProxyContext, Target } from '../../../types'
import { Proxy } from './Proxy'

export class LockableProxy extends Proxy {
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
