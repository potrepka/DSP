import type { ProxyContext } from '../../../types'
import { InputProxy, ProducerProxy } from '../core'

export class PhasorProxy extends ProducerProxy {
  constructor(context: ProxyContext, id: string) {
    super(context, id, 'Phasor')
  }
  getFrequency = (): InputProxy => this.createInputProxy('Frequency')
  getMode = (): InputProxy => this.createInputProxy('Mode')
  getReset = (): InputProxy => this.createInputProxy('Reset')
}
