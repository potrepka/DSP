import { Interpolation } from '../../../enums/global'
import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BufferProxy, InputProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'
import { VectorProxy } from '../core/VectorProxy'

export class TableShaperProxy extends TransformerProxy {
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
  getInputInterpolation = (): Chainable<Interpolation> => {
    return chainable(this.call('getInputInterpolation', []))
  }
  setInputInterpolation = (interpolation: Interpolation): Chainable<void> => {
    return chainable(this.call('setInputInterpolation', [interpolation]))
  }
  getPositionInterpolation = (): Chainable<Interpolation> => {
    return chainable(this.call('getPositionInterpolation', []))
  }
  setPositionInterpolation = (
    interpolation: Interpolation,
  ): Chainable<void> => {
    return chainable(this.call('setPositionInterpolation', [interpolation]))
  }
  getPosition = (): Chainable<InputProxy> => this.createInput('Position')
}
