import { ClipperMode } from '../../../enums/node'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class ClipperProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<ClipperMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: ClipperMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getMin = (): Chainable<InputProxy> => this.createInput('Min')
  getMax = (): Chainable<InputProxy> => this.createInput('Max')
}
