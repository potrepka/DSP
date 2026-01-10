import { PhasorMode } from '../../../enums/node'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class PhasorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<PhasorMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: PhasorMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getFrequency = (): Chainable<InputProxy> => this.createInput('Frequency')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
}
