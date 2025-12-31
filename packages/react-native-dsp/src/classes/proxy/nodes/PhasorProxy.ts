import { Target } from '../../../types/module'
import { ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class PhasorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getFrequency = (): Promise<InputProxy> => this.createInput('Frequency')
  getMode = (): Promise<InputProxy> => this.createInput('Mode')
  getReset = (): Promise<InputProxy> => this.createInput('Reset')
}
