import type { NodeType, ProxyContext } from '../../../types'
import { OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class ProducerProxy extends NodeProxy {
  constructor(context: ProxyContext, id: string, nodeType: NodeType) {
    super(context, id, nodeType)
  }
  getOutput = (): OutputProxy => {
    return this.createOutputProxy('Output')
  }
}
