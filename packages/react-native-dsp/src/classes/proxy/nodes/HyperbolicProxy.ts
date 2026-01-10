import { HyperbolicMode } from '../../../enums/node'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class HyperbolicProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<HyperbolicMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: HyperbolicMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
}
