import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BufferProxy, InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'
import { VectorProxy } from '../core/VectorProxy'

export class SamplePlayerProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getSamples = (): Chainable<VectorProxy<BufferProxy>> => {
    return chainable(
      this.call<Target>('getSamples', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new BufferProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  getSpeed = (): Chainable<InputProxy> => this.createInput('Speed')
  getStartTime = (): Chainable<InputProxy> => this.createInput('StartTime')
  getSampleIndex = (): Chainable<InputProxy> => this.createInput('SampleIndex')
  getInterpolation = (): Chainable<InputProxy> =>
    this.createInput('Interpolation')
  getGate = (): Chainable<InputProxy> => this.createInput('Gate')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
  getCurrentTime = (): Chainable<number> => {
    return chainable(this.call('getCurrentTime', []))
  }
}
