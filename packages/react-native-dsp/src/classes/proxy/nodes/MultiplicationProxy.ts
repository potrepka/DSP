import { Target } from '../../../types/module'
import { ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class MultiplicationProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getFactor = (): Promise<InputProxy> => this.createInput('Factor')
}
