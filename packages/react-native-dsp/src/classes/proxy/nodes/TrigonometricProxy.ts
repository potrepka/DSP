import { TrigonometricMode } from '../../../enums/node'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class TrigonometricProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<TrigonometricMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: TrigonometricMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
}
