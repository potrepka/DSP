import type { ProxyContext, Target } from '../../../types'
import { InputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class ConsumerProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getInput = (): Promise<InputProxy> => this.createInput('Input')
}
