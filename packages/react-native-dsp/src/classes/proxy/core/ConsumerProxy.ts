import type { NodeType, ProxyContext } from '../../../types'
import { InputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class ConsumerProxy extends NodeProxy {
  constructor(context: ProxyContext, id: string, nodeType: NodeType) {
    super(context, id, nodeType)
  }
  getInput = (): InputProxy => {
    return this.createInputProxy('Input')
  }
}
