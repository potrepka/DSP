import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy, OutputProxy } from '../../core/BufferProxy'
import { TransformerProxy } from '../../core/TransformerProxy'

export class DifferentiatorProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getGate = (): Chainable<OutputProxy> => this.createOutput('Gate')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
}
