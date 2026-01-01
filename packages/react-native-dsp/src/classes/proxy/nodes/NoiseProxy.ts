import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class NoiseProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
}
