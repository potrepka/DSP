import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
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
  toRealImaginary = (
    _input: unknown,
    _real: unknown,
    _imaginary: unknown,
  ): never => {
    throw new Error('toRealImaginary is not implemented')
  }
  fromRealImaginary = (
    _real: unknown,
    _imaginary: unknown,
    _output: unknown,
  ): never => {
    throw new Error('fromRealImaginary is not implemented')
  }
  toMagnitudePhase = (
    _input: unknown,
    _magnitude: unknown,
    _phase: unknown,
  ): never => {
    throw new Error('toMagnitudePhase is not implemented')
  }
  fromMagnitudePhase = (
    _magnitude: unknown,
    _phase: unknown,
    _output: unknown,
  ): never => {
    throw new Error('fromMagnitudePhase is not implemented')
  }
}
