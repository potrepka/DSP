import { ComparisonMode } from '../../../../enums/node'
import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { TransformerProxy } from '../../core/TransformerProxy'

export class ComparisonProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<ComparisonMode> => {
    return chainable(this.call('getMode', []))
  }
  setMode = (mode: ComparisonMode): Chainable<void> => {
    return chainable(this.call('setMode', [mode]))
  }
  getThreshold = (): Chainable<InputProxy> => this.createInput('Threshold')
}
