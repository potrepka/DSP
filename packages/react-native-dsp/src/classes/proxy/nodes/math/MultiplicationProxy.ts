import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { TransformerProxy } from '../../core/TransformerProxy'

export class MultiplicationProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getFactor = (): Chainable<InputProxy> => this.createInput('Factor')
}
