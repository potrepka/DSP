import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy, OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class TransformerProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getInput = (): Chainable<InputProxy> => this.createInput('Input')
  getOutput = (): Chainable<OutputProxy> => this.createOutput('Output')
}
