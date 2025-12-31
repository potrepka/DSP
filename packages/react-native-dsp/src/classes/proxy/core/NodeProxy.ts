import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'
import { InputProxy, OutputProxy } from './BufferProxy'
import { VectorProxy } from './VectorProxy'

export class NodeProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  protected createInput = (inputName: string): Chainable<InputProxy> => {
    return chainable(
      this.call<Target>(`get${inputName}`, []).then(
        (target) => new InputProxy(this.context, target),
      ),
    )
  }
  protected createOutput = (outputName: string): Chainable<OutputProxy> => {
    return chainable(
      this.call<Target>(`get${outputName}`, []).then(
        (target) => new OutputProxy(this.context, target),
      ),
    )
  }
  isActive = (): Chainable<boolean> => {
    return chainable(this.call('isActive', []))
  }
  setActive = (active: boolean): Chainable<void> => {
    return chainable(this.call('setActive', [active]))
  }
  getNumChannels = (): Chainable<number> => {
    return chainable(this.call('getNumChannels', []))
  }
  setNumChannels = (numChannels: number): Chainable<void> => {
    return chainable(this.call('setNumChannels', [numChannels]))
  }
  getNumInputChannels = (): Chainable<number> => {
    return chainable(this.call('getNumInputChannels', []))
  }
  setNumInputChannels = (numChannels: number): Chainable<void> => {
    return chainable(this.call('setNumInputChannels', [numChannels]))
  }
  getNumOutputChannels = (): Chainable<number> => {
    return chainable(this.call('getNumOutputChannels', []))
  }
  setNumOutputChannels = (numChannels: number): Chainable<void> => {
    return chainable(this.call('setNumOutputChannels', [numChannels]))
  }
  getNumSamples = (): Chainable<number> => {
    return chainable(this.call('getNumSamples', []))
  }
  setNumSamples = (numSamples: number): Chainable<void> => {
    return chainable(this.call('setNumSamples', [numSamples]))
  }
  getSampleRate = (): Chainable<number> => {
    return chainable(this.call('getSampleRate', []))
  }
  setSampleRate = (sampleRate: number): Chainable<void> => {
    return chainable(this.call('setSampleRate', [sampleRate]))
  }
  getOneOverNumSamples = (): Chainable<number> => {
    return chainable(this.call('getOneOverNumSamples', []))
  }
  getOneOverSampleRate = (): Chainable<number> => {
    return chainable(this.call('getOneOverSampleRate', []))
  }
  getInputs = (): Chainable<VectorProxy<InputProxy>> => {
    return chainable(
      this.call<Target>('getInputs', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new InputProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  getOutputs = (): Chainable<VectorProxy<OutputProxy>> => {
    return chainable(
      this.call<Target>('getOutputs', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new OutputProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  getChildren = (): Chainable<VectorProxy<NodeProxy>> => {
    return chainable(
      this.call<Target>('getChildren', []).then(
        (target) =>
          new VectorProxy(this.context, target, (itemTarget: Target) => {
            return new NodeProxy(this.context, itemTarget)
          }),
      ),
    )
  }
  addChild = (child: NodeProxy): Chainable<void> => {
    return chainable(this.call('addChild', [child]))
  }
  removeChild = (child: NodeProxy): Chainable<void> => {
    return chainable(this.call('removeChild', [child]))
  }
  sortChildren = (): Chainable<void> => {
    return chainable(this.call('sortChildren', []))
  }
  disconnectAll = (): Chainable<void> => {
    return chainable(this.call('disconnectAll', []))
  }
  process = (): Chainable<void> => {
    return chainable(this.call('process', []))
  }
}
