import { NoiseMode } from '../../../enums/node'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class NoiseProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<NoiseMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: NoiseMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
}
