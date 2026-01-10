import { ShaperMode } from '../../../enums/node'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class ShaperProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<ShaperMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: ShaperMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getDrive = (): Chainable<InputProxy> => this.createInput('Drive')
  getOutputSample = (channel: number, input: number): Chainable<number> => {
    return chainable(this.call('getOutputSample', [channel, input]))
  }
}
