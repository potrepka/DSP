import type { ProxyContext, Target } from '../../../types'
import { Proxy } from './Proxy'

export class NormalizedFFTProxy extends Proxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }

  setup = (size: number): Promise<void> => {
    return this.call('setup', [size])
  }

  getSize = (): Promise<number> => {
    return this.call('getSize', [])
  }

  getComplexSize = (): Promise<number> => {
    return this.call('getComplexSize', [])
  }
}
