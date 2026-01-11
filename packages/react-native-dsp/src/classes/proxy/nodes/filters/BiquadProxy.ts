import { BiquadMode } from '../../../../enums/node'
import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { TransformerProxy } from '../../core/TransformerProxy'

export class BiquadProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<BiquadMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: BiquadMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getFrequency = (): Chainable<InputProxy> => this.createInput('Frequency')
  getResonance = (): Chainable<InputProxy> => this.createInput('Resonance')
  getAmplitude = (): Chainable<InputProxy> => this.createInput('Amplitude')
  getFrequencyResponse = (frequency: number): Chainable<number> => {
    return chainable(this.call('getFrequencyResponse', [frequency]))
  }
}
