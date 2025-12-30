import type { ProxyContext } from '../../../types'
import { InputProxy, TransformerProxy } from '../core'

export class BiquadProxy extends TransformerProxy {
  constructor(context: ProxyContext, id: string) {
    super(context, id, 'Biquad')
  }
  getFrequency = (): InputProxy => this.createInputProxy('Frequency')
  getResonance = (): InputProxy => this.createInputProxy('Resonance')
  getAmplitude = (): InputProxy => this.createInputProxy('Amplitude')
  getMode = (): InputProxy => this.createInputProxy('Mode')
  getFrequencyResponse = (frequency: number): Promise<number> => {
    return this.context.call(this.toTarget(), 'getFrequencyResponse', [
      frequency,
    ])
  }
}
