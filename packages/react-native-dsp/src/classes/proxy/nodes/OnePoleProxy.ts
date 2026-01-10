import { OnePoleMode } from '../../../enums/node'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class OnePoleProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<OnePoleMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: OnePoleMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getFrequency = (): Chainable<InputProxy> => this.createInput('Frequency')
}
