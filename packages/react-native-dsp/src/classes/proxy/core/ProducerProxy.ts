import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class ProducerProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getOutput = (): Chainable<OutputProxy> => this.createOutput('Output')
}
