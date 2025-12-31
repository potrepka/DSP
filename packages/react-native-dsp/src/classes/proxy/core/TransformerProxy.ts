import type { ProxyContext, Target } from '../../../types'
import { InputProxy, OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'

export class TransformerProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getInput = (): Promise<InputProxy> => this.createInput('Input')
  getOutput = (): Promise<OutputProxy> => this.createOutput('Output')
}
