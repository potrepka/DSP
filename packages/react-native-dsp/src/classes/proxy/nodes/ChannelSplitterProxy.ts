import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { OutputProxy } from '../core/BufferProxy'
import { ConsumerProxy } from '../core/ConsumerProxy'

export class ChannelSplitterProxy extends ConsumerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getOutput = (channel: number): Chainable<OutputProxy> => {
    return chainable(
      this.call<Target>('getOutput', [channel]).then(
        (target) => new OutputProxy(this.context, target),
      ),
    )
  }
}
