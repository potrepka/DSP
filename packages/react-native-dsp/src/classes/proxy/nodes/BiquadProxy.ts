import type { ProxyContext, Target } from '../../../types'
import { InputProxy, TransformerProxy } from '../core'

export class BiquadProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getFrequency = (): Promise<InputProxy> => this.createInput('Frequency')
  getResonance = (): Promise<InputProxy> => this.createInput('Resonance')
  getAmplitude = (): Promise<InputProxy> => this.createInput('Amplitude')
  getMode = (): Promise<InputProxy> => this.createInput('Mode')

  getFrequencyResponse = (frequency: number): Promise<number> => {
    return this.call('getFrequencyResponse', [frequency])
  }
}
