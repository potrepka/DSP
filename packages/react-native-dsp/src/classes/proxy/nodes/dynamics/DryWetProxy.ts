import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy, OutputProxy } from '../../core/BufferProxy'
import { ProducerProxy } from '../../core/ProducerProxy'

export class DryWetProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getDry = (): Chainable<InputProxy> => this.createInput('Dry')
  getWet = (): Chainable<InputProxy> => this.createInput('Wet')
  getMixAmount = (): Chainable<InputProxy> => this.createInput('MixAmount')
  getA = (): Chainable<OutputProxy> => this.createOutput('A')
  getB = (): Chainable<OutputProxy> => this.createOutput('B')
}
