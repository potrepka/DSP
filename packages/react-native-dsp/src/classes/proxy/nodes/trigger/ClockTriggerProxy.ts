import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { ProducerProxy } from '../../core/ProducerProxy'

export class ClockTriggerProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getInterval = (): Chainable<InputProxy> => this.createInput('Interval')
  getDelayTime = (): Chainable<InputProxy> => this.createInput('DelayTime')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
  getCurrentTime = (): Chainable<number> => {
    return chainable(this.call('getCurrentTime', []))
  }
}
