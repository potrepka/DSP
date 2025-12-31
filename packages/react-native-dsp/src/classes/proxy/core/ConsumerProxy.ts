import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class ConsumerProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getInput = (): Chainable<InputProxy> => this.createInput('Input')
}
