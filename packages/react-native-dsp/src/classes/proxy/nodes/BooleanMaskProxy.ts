import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class BooleanMaskProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMask = (): Chainable<InputProxy> => this.createInput('Mask')
}
