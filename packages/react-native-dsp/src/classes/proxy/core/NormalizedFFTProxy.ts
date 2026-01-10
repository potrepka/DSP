import { chainable } from '../../../helpers/proxy'
import { MagnitudePhase, RealImaginary, Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'

export class NormalizedFFTProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  setup = (size: number): Chainable<void> => {
    return chainable(this.call('setup', [size]))
  }
  getSize = (): Chainable<number> => {
    return chainable(this.call('getSize', []))
  }
  getComplexSize = (): Chainable<number> => {
    return chainable(this.call('getComplexSize', []))
  }
  toRealImaginary = (time: number[]): Chainable<RealImaginary> => {
    return chainable(this.call('toRealImaginary', [time]))
  }
  fromRealImaginary = (
    real: number[],
    imaginary: number[],
  ): Chainable<number[]> => {
    return chainable(this.call('fromRealImaginary', [real, imaginary]))
  }
  toMagnitudePhase = (time: number[]): Chainable<MagnitudePhase> => {
    return chainable(this.call('toMagnitudePhase', [time]))
  }
  fromMagnitudePhase = (
    magnitude: number[],
    phase: number[],
  ): Chainable<number[]> => {
    return chainable(this.call('fromMagnitudePhase', [magnitude, phase]))
  }
}
