import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class ShaperProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getDrive = (): Chainable<InputProxy> => this.createInput('Drive')
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
  getOutputSignal = (channel: number, input: number): Chainable<number> => {
    return chainable(this.call('getOutputSignal', [channel, input]))
  }
}
