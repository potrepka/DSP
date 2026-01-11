import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { TransformerProxy } from '../../core/TransformerProxy'

export class TriggerHoldProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getHoldTime = (): Chainable<InputProxy> => this.createInput('HoldTime')
  getCurrentTime = (): Chainable<number> => {
    return chainable(this.call('getCurrentTime', []))
  }
}
