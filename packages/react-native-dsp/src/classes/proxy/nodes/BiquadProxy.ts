import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class BiquadProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getFrequency = (): Chainable<InputProxy> => this.createInput('Frequency')
  getResonance = (): Chainable<InputProxy> => this.createInput('Resonance')
  getAmplitude = (): Chainable<InputProxy> => this.createInput('Amplitude')
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
  getFrequencyResponse = (frequency: number): Chainable<number> => {
    return chainable(this.call('getFrequencyResponse', [frequency]))
  }
}
