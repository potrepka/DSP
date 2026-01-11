import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy, OutputProxy } from '../../core/BufferProxy'
import { ConsumerProxy } from '../../core/ConsumerProxy'

export class MidSideProxy extends ConsumerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMixAmount = (): Chainable<InputProxy> => this.createInput('MixAmount')
  getMid = (): Chainable<OutputProxy> => this.createOutput('Mid')
  getSide = (): Chainable<OutputProxy> => this.createOutput('Side')
}
