import { ReservedKeyword, TargetType } from '../../../enums'
import type { Data, MidiBuffer, ProxyContext, Target } from '../../../types'
import { InputProxy, OutputProxy } from './BufferProxy'
import { NodeProxy } from './NodeProxy'
import { VectorProxy } from './VectorProxy'

export class NodeProcessorProxy {
  constructor(private readonly context: ProxyContext) {}
  toTarget = (): Target => ({ type: TargetType.NodeProcessor })
  delete = (): Promise<void> => {
    this.context.port.postMessage({ message: 'delete' })
    return Promise.resolve()
  }
  isActive = (): Promise<boolean> => {
    return this.context.call(this.toTarget(), 'isActive', [])
  }
  setActive = (active: boolean): Promise<void> => {
    return this.context.call(this.toTarget(), 'setActive', [active])
  }
  getAudioInput = (): OutputProxy => {
    return new OutputProxy(
      this.context,
      ReservedKeyword.NodeProcessor,
      'AudioInput',
    )
  }
  getAudioOutput = (): InputProxy => {
    return new InputProxy(
      this.context,
      ReservedKeyword.NodeProcessor,
      'AudioOutput',
    )
  }
  getAudioInputClipping = (): OutputProxy => {
    return new OutputProxy(
      this.context,
      ReservedKeyword.NodeProcessor,
      'AudioInputClipping',
    )
  }
  getAudioOutputClipping = (): OutputProxy => {
    return new OutputProxy(
      this.context,
      ReservedKeyword.NodeProcessor,
      'AudioOutputClipping',
    )
  }
  getNumInputChannels = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getNumInputChannels', [])
  }
  setNumInputChannels = (numChannels: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setNumInputChannels', [
      numChannels,
    ])
  }
  getNumOutputChannels = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getNumOutputChannels', [])
  }
  setNumOutputChannels = (numChannels: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setNumOutputChannels', [
      numChannels,
    ])
  }
  getNumSamples = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getNumSamples', [])
  }
  setNumSamples = (numSamples: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setNumSamples', [numSamples])
  }
  getSampleRate = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getSampleRate', [])
  }
  setSampleRate = (sampleRate: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setSampleRate', [sampleRate])
  }
  setInputSize = (numChannels: number, numSamples: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setInputSize', [
      numChannels,
      numSamples,
    ])
  }
  setOutputSize = (numChannels: number, numSamples: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setOutputSize', [
      numChannels,
      numSamples,
    ])
  }
  getDefaultNode = (): NodeProxy => {
    return new NodeProxy(this.context, ReservedKeyword.DefaultNode, 'Identity')
  }
  getNodes = (): VectorProxy<NodeProxy> => {
    const target: Target = {
      type: TargetType.Vector,
      id: `${ReservedKeyword.NodeProcessor}:Nodes`,
    }
    return new VectorProxy(this.context, target, (nodeTarget: Target) => {
      if (nodeTarget.type !== TargetType.Node) {
        throw new Error('Expected Node target')
      }
      return new NodeProxy(this.context, nodeTarget.id, nodeTarget.nodeType)
    })
  }
  getInputMessages = (): MidiBuffer => {
    throw new Error(
      'getInputMessages returns a complex object that cannot be proxied',
    )
  }
  getOutputMessages = (): MidiBuffer => {
    throw new Error(
      'getOutputMessages returns a complex object that cannot be proxied',
    )
  }
  process = (_audioBuffer: Data, _midiBuffer: MidiBuffer): Promise<void> => {
    throw new Error(
      'process requires Data and MidiBuffer objects from the worklet',
    )
  }
}
