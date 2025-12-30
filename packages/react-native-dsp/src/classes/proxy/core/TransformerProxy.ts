import type { NodeType, ProxyContext } from '../../../types'
import { InputProxy, OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class TransformerProxy extends NodeProxy {
  constructor(context: ProxyContext, id: string, nodeType: NodeType) {
    super(context, id, nodeType)
  }
  getInput = (): InputProxy => {
    return this.createInputProxy('Input')
  }
  getOutput = (): OutputProxy => {
    return this.createOutputProxy('Output')
  }
}
