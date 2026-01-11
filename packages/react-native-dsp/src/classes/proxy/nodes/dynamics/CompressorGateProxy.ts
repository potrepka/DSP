import { chainable } from '../../../../helpers/proxy'
import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy } from '../../core/BufferProxy'
import { TransformerProxy } from '../../core/TransformerProxy'

export class CompressorGateProxy extends TransformerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getControl = (): Chainable<InputProxy> => this.createInput('Control')
  getThreshold = (): Chainable<InputProxy> => this.createInput('Threshold')
  getSoftness = (): Chainable<InputProxy> => this.createInput('Softness')
  getCompressionRatio = (): Chainable<InputProxy> =>
    this.createInput('CompressionRatio')
  getGateRatio = (): Chainable<InputProxy> => this.createInput('GateRatio')
  getAttack = (): Chainable<InputProxy> => this.createInput('Attack')
  getRelease = (): Chainable<InputProxy> => this.createInput('Release')
  getGain = (): Chainable<InputProxy> => this.createInput('Gain')
  getGainResponse = (channel: number, input: number): Chainable<number> => {
    return chainable(this.call('getGainResponse', [channel, input]))
  }
}
