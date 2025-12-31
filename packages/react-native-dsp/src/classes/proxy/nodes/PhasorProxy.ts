import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class PhasorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getFrequency = (): Chainable<InputProxy> => this.createInput('Frequency')
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
}
