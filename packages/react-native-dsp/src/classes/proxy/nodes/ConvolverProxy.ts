import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BufferProxy } from '../core/BufferProxy'
import { TransformerProxy } from '../core/TransformerProxy'

export class ConvolverProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getHeadSize = (): Chainable<number> => {
    return chainable(this.call('getHeadSize', []))
  }
  setHeadSize = (headSize: number): Chainable<void> => {
    return chainable(this.call('setHeadSize', [headSize]))
  }
  getTailSize = (): Chainable<number> => {
    return chainable(this.call('getTailSize', []))
  }
  setTailSize = (tailSize: number): Chainable<void> => {
    return chainable(this.call('setTailSize', [tailSize]))
  }
  getBuffer = (): Chainable<BufferProxy> => {
    return chainable(
      this.call<Target>('getBuffer', []).then(
        (target) => new BufferProxy(this.context, target),
      ),
    )
  }
  setBuffer = (buffer: BufferProxy): Chainable<void> => {
    return chainable(this.call('setBuffer', [buffer]))
  }
  initConvolvers = (): Chainable<void> => {
    return chainable(this.call('initConvolvers', []))
  }
}
