import type { ProxyContext, Target } from '../../../types'
import { InputProxy, TransformerProxy } from '../core'

export class MultiplicationProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getFactor = (): Promise<InputProxy> => this.createInput('Factor')
}
