import { Target } from '../../../types/module'
import { ProxyContext } from '../../../types/proxy'
import { InputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class ConsumerProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getInput = (): Promise<InputProxy> => this.createInput('Input')
}
