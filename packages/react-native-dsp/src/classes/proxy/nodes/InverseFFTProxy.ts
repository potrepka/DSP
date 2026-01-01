import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class InverseFFTProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMagnitude = (): Chainable<InputProxy> => this.createInput('Magnitude')
  getPhase = (): Chainable<InputProxy> => this.createInput('Phase')
}
