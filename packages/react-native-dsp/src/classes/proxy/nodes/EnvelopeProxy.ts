import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class EnvelopeProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getAttack = (): Chainable<InputProxy> => this.createInput('Attack')
  getRelease = (): Chainable<InputProxy> => this.createInput('Release')
  getAttackShape = (): Chainable<InputProxy> => this.createInput('AttackShape')
  getReleaseShape = (): Chainable<InputProxy> =>
    this.createInput('ReleaseShape')
  getGate = (): Chainable<InputProxy> => this.createInput('Gate')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
  getCurrentTime = (): Chainable<number> => {
    return chainable(this.call('getCurrentTime', []))
  }
}
