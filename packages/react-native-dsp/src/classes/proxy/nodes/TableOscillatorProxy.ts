import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BufferProxy, InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'
import { VectorProxy } from '../core/VectorProxy'

export class TableOscillatorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getTables = (): Chainable<VectorProxy<BufferProxy>> => {
    return chainable(
      this.call<Target>('getTables', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new BufferProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  getPhase = (): Chainable<InputProxy> => this.createInput('Phase')
  getPosition = (): Chainable<InputProxy> => this.createInput('Position')
  getPhaseInterpolation = (): Chainable<InputProxy> =>
    this.createInput('PhaseInterpolation')
  getPositionInterpolation = (): Chainable<InputProxy> =>
    this.createInput('PositionInterpolation')
}
