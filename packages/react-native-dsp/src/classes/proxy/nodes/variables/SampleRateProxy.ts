import { Target } from '../../../../types/module'
import { ProxyContext } from '../../../../types/proxy'
import { ProducerProxy } from '../../core/ProducerProxy'

export class SampleRateProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
}
