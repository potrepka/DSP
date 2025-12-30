import type { ProxyContext } from '../../../types'
import { InputProxy, TransformerProxy } from '../core'

export class MultiplicationProxy extends TransformerProxy {
  constructor(context: ProxyContext, id: string) {
    super(context, id, 'Multiplication')
  }
  getFactor = (): InputProxy => this.createInputProxy('Factor')
}
