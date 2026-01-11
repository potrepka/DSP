import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { OutputProxy } from '../../core/BufferProxy'
import { ConsumerProxy } from '../../core/ConsumerProxy'

export class ForwardFFTProxy extends ConsumerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMagnitude = (): Chainable<OutputProxy> => this.createOutput('Magnitude')
  getPhase = (): Chainable<OutputProxy> => this.createOutput('Phase')
}
