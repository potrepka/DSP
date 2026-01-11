import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy, OutputProxy } from '../../core/BufferProxy'
import { ConsumerProxy } from '../../core/ConsumerProxy'

export class StereoPannerProxy extends ConsumerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getDirection = (): Chainable<InputProxy> => this.createInput('Direction')
  getLeft = (): Chainable<OutputProxy> => this.createOutput('Left')
  getRight = (): Chainable<OutputProxy> => this.createOutput('Right')
}
