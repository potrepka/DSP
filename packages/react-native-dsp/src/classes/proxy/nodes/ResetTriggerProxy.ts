import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class ResetTriggerProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  reset = (): Chainable<void> => {
    return chainable(this.call('reset', []))
  }
  resetChannel = (channel: number): Chainable<void> => {
    return chainable(this.call('resetChannel', [channel]))
  }
}
