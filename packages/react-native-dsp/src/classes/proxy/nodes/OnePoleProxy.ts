import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class OnePoleProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getFrequency = (): Chainable<InputProxy> => this.createInput('Frequency')
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
}
