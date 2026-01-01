import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class TrigonometricProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
}
