import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class FrequencyToNoteProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getTuningFrequency = (): Chainable<InputProxy> =>
    this.createInput('TuningFrequency')
}
