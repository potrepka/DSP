import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class FunctionOscillatorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getPhase = (): Chainable<InputProxy> => this.createInput('Phase')
}
