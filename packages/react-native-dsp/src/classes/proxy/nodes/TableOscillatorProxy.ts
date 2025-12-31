import { TargetType } from '../../../enums'
import type { ProxyContext, Target } from '../../../types'
import { BufferProxy, InputProxy, ProducerProxy, VectorProxy } from '../core'

export class TableOscillatorProxy extends ProducerProxy {
  constructor(context: ProxyContext, id: string) {
    super(context, id, 'TableOscillator')
  }
  getTables = (): VectorProxy<BufferProxy> => {
    const target: Target = {
      type: TargetType.BufferVector,
      id: `${this.id}:Tables`,
    }
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      if (itemTarget.type !== TargetType.Buffer) {
        throw new Error('Expected Buffer target')
      }
      return new BufferProxy(this.context, itemTarget.id)
    })
  }
  getPhase = (): InputProxy => this.createInputProxy('Phase')
  getPosition = (): InputProxy => this.createInputProxy('Position')
  getPhaseInterpolation = (): InputProxy =>
    this.createInputProxy('PhaseInterpolation')
  getPositionInterpolation = (): InputProxy =>
    this.createInputProxy('PositionInterpolation')
}
