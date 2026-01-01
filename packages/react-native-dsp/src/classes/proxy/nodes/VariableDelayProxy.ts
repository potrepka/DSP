import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy, OutputProxy } from '../core/BufferProxy'
import { NodeProxy } from '../core/NodeProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class VariableDelayProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMaxDelayTime = (): Chainable<number> => {
    return chainable(this.call('getMaxDelayTime', []))
  }
  setMaxDelayTime = (maxDelayTime: number): Chainable<void> => {
    return chainable(this.call('setMaxDelayTime', [maxDelayTime]))
  }
  getDelayTime = (): Chainable<InputProxy> => this.createInput('DelayTime')
  getDecayTime = (): Chainable<InputProxy> => this.createInput('DecayTime')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
  getFeedbackSource = (): Chainable<OutputProxy> =>
    this.createOutput('FeedbackSource')
  getFeedbackSink = (): Chainable<InputProxy> =>
    this.createInput('FeedbackSink')
  getFeedbackProcessor = (): Chainable<NodeProxy> => {
    return chainable(
      this.call<Target>('getFeedbackProcessor', []).then(
        (target) => new NodeProxy(this.context, target),
      ),
    )
  }
}
