import { Target } from '../../../types/module'
import { ProxyContext } from '../../../types/proxy'
import { BufferProxy, InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'
import { VectorProxy } from '../core/VectorProxy'

export class TableOscillatorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  getTables = async (): Promise<VectorProxy<BufferProxy>> => {
    const target = await this.call<Target>('getTables', [])
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      return new BufferProxy(this.context, itemTarget)
    })
  }

  getPhase = (): Promise<InputProxy> => this.createInput('Phase')
  getPosition = (): Promise<InputProxy> => this.createInput('Position')
  getPhaseInterpolation = (): Promise<InputProxy> =>
    this.createInput('PhaseInterpolation')
  getPositionInterpolation = (): Promise<InputProxy> =>
    this.createInput('PositionInterpolation')
}
