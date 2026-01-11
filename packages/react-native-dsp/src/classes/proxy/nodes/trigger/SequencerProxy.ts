import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { BufferProxy, InputProxy } from '../../core/BufferProxy'
import { ProducerProxy } from '../../core/ProducerProxy'
import { VectorProxy } from '../../core/VectorProxy'

export class SequencerProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getSequences = (): Chainable<VectorProxy<BufferProxy>> => {
    return chainable(
      this.call<Target>('getSequences', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new BufferProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  getSequenceIndex = (): Chainable<InputProxy> =>
    this.createInput('SequenceIndex')
  getPositionIndex = (): Chainable<InputProxy> =>
    this.createInput('PositionIndex')
}
