import { Target } from '../../../types/module'
import { ProxyContext } from '../../../types/proxy'
import { OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class ProducerProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getOutput = (): Promise<OutputProxy> => this.createOutput('Output')
}
