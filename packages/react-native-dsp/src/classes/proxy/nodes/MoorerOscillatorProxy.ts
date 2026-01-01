import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { InputProxy } from '../core/BufferProxy'
import { ProducerProxy } from '../core/ProducerProxy'

export class MoorerOscillatorProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getPhase = (): Chainable<InputProxy> => this.createInput('Phase')
  getIntensity = (): Chainable<InputProxy> => this.createInput('Intensity')
  getModulationIndex = (): Chainable<InputProxy> =>
    this.createInput('ModulationIndex')
  getHarmonics = (): Chainable<InputProxy> => this.createInput('Harmonics')
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
}
