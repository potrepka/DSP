import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { ProducerProxy } from '../../core/ProducerProxy'

export class OnOffProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getOnTrigger = (): Chainable<InputProxy> => this.createInput('OnTrigger')
  getOffTrigger = (): Chainable<InputProxy> => this.createInput('OffTrigger')
}
