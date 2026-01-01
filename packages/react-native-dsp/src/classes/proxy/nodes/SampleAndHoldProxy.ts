import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class SampleAndHoldProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getGate = (): Chainable<InputProxy> => this.createInput('Gate')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
}
