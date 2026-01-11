import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { ProducerProxy } from '../../core/ProducerProxy'

export class ChannelMergerProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getInput = (channel: number): Chainable<InputProxy> => {
    return chainable(
      this.call<Target>('getInput', [channel]).then(
        (target) => new InputProxy(this.context, target),
      ),
    )
  }
}
