import { Shape } from '../../../../enums/global'
import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { ProducerProxy } from '../../core/ProducerProxy'

export class EnvelopeProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getAttackShape = (): Chainable<Shape> => {
    return chainable(this.call('getAttackShape', []))
  }
  setAttackShape = (shape: Shape): Chainable<void> => {
    return chainable(this.call('setAttackShape', [shape]))
  }
  getReleaseShape = (): Chainable<Shape> => {
    return chainable(this.call('getReleaseShape', []))
  }
  setReleaseShape = (shape: Shape): Chainable<void> => {
    return chainable(this.call('setReleaseShape', [shape]))
  }
  getAttack = (): Chainable<InputProxy> => this.createInput('Attack')
  getRelease = (): Chainable<InputProxy> => this.createInput('Release')
  getGate = (): Chainable<InputProxy> => this.createInput('Gate')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
  getCurrentTime = (): Chainable<number> => {
    return chainable(this.call('getCurrentTime', []))
  }
}
