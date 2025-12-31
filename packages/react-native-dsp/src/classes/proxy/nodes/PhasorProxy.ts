import type { ProxyContext, Target } from '../../../types'
import { InputProxy, ProducerProxy } from '../core'

export class PhasorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getFrequency = (): Promise<InputProxy> => this.createInput('Frequency')
  getMode = (): Promise<InputProxy> => this.createInput('Mode')
  getReset = (): Promise<InputProxy> => this.createInput('Reset')
}
