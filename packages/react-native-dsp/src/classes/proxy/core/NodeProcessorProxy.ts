import { chainable } from '../../../helpers/proxy'
import { Data, MidiBuffer, Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from './BaseProxy'
import { InputProxy, OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'
import { VectorProxy } from './VectorProxy'

export class NodeProcessorProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  isActive = (): Chainable<boolean> => {
    return chainable(this.call('isActive', []))
  }
  setActive = (active: boolean): Chainable<void> => {
    return chainable(this.call('setActive', [active]))
  }
  getAudioInput = (): Chainable<OutputProxy> => {
    return chainable(
      this.call<Target>('getAudioInput', []).then(
        (target) => new OutputProxy(this.context, target),
      ),
    )
  }
  getAudioOutput = (): Chainable<InputProxy> => {
    return chainable(
      this.call<Target>('getAudioOutput', []).then(
        (target) => new InputProxy(this.context, target),
      ),
    )
  }
  getAudioInputClipping = (): Chainable<OutputProxy> => {
    return chainable(
      this.call<Target>('getAudioInputClipping', []).then(
        (target) => new OutputProxy(this.context, target),
      ),
    )
  }
  getAudioOutputClipping = (): Chainable<OutputProxy> => {
    return chainable(
      this.call<Target>('getAudioOutputClipping', []).then(
        (target) => new OutputProxy(this.context, target),
      ),
    )
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
  setInputSize = (numChannels: number, numSamples: number): Chainable<void> => {
    return chainable(this.call('setInputSize', [numChannels, numSamples]))
  }
  setOutputSize = (
    numChannels: number,
    numSamples: number,
  ): Chainable<void> => {
    return chainable(this.call('setOutputSize', [numChannels, numSamples]))
  }
  getDefaultNode = (): Chainable<NodeProxy> => {
    return chainable(
      this.call<Target>('getDefaultNode', []).then(
        (target) => new NodeProxy(this.context, target),
      ),
    )
  }
  getNodes = (): Chainable<VectorProxy<NodeProxy>> => {
    return chainable(
      this.call<Target>('getNodes', []).then(
        (target) =>
          new VectorProxy(this.context, target, (nodeTarget: Target) => {
            return new NodeProxy(this.context, nodeTarget)
          }),
      ),
    )
  }
  getInputMessages = (): never => {
    throw new Error('getInputMessages is not implemented')
  }
  getOutputMessages = (): never => {
    throw new Error('getOutputMessages is not implemented')
  }
  process = (_audioBuffer: Data, _midiBuffer: MidiBuffer): never => {
    throw new Error('process is not implemented')
  }
}
