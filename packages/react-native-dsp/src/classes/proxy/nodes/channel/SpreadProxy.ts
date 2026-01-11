import { SpreadMode } from '../../../../enums/node'
import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { TransformerProxy } from '../../core/TransformerProxy'

export class SpreadProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<SpreadMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: SpreadMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getSpread = (): Chainable<InputProxy> => this.createInput('Spread')
}
