import { Target } from '../../../types/module'
import { ProxyContext } from '../../../types/proxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class ReciprocalProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
}
