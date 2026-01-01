import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class ClipperProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMin = (): Chainable<InputProxy> => this.createInput('Min')
  getMax = (): Chainable<InputProxy> => this.createInput('Max')
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
}
