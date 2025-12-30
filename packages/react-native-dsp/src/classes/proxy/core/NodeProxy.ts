import { TargetType } from '../../../enums'
import type { NodeType, ProxyContext, Target } from '../../../types'
import { InputProxy, OutputProxy } from './BufferProxy'
import { VectorProxy } from './VectorProxy'

export class NodeProxy {
  constructor(
    protected readonly context: ProxyContext,
    public readonly id: string,
    public readonly nodeType: NodeType,
  ) {}
  toTarget = (): Target => ({
    type: TargetType.Node,
    nodeType: this.nodeType,
    id: this.id,
  })
  delete = (): Promise<void> => {
    this.context.port.postMessage({
      message: 'deleteNode',
      nodeId: this.id,
    })
    return Promise.resolve()
  }
  protected createInputProxy = (inputName: string) => {
    return new InputProxy(this.context, this.id, inputName)
  }
  protected createOutputProxy = (outputName: string) => {
    return new OutputProxy(this.context, this.id, outputName)
  }
  isActive = (): Promise<boolean> => {
    return this.context.call(this.toTarget(), 'isActive', [])
  }
  setActive = (active: boolean): Promise<void> => {
    return this.context.call(this.toTarget(), 'setActive', [active])
  }
  getNumChannels = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getNumChannels', [])
  }
  setNumChannels = (numChannels: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setNumChannels', [numChannels])
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
  getOneOverNumSamples = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getOneOverNumSamples', [])
  }
  getOneOverSampleRate = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getOneOverSampleRate', [])
  }
  getInputs = (): VectorProxy<InputProxy> => {
    const target: Target = { type: TargetType.Vector, id: `${this.id}:Inputs` }
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      if (itemTarget.type !== TargetType.Input) {
        throw new Error('Expected Input target')
      }
      const [nodeId, inputName] = itemTarget.id.split(':')
      return new InputProxy(this.context, nodeId, inputName)
    })
  }
  getOutputs = (): VectorProxy<OutputProxy> => {
    const target: Target = { type: TargetType.Vector, id: `${this.id}:Outputs` }
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      if (itemTarget.type !== TargetType.Output) {
        throw new Error('Expected Output target')
      }
      const [nodeId, outputName] = itemTarget.id.split(':')
      return new OutputProxy(this.context, nodeId, outputName)
    })
  }
  getChildren = (): VectorProxy<NodeProxy> => {
    const target: Target = {
      type: TargetType.Vector,
      id: `${this.id}:Children`,
    }
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      if (itemTarget.type !== TargetType.Node) {
        throw new Error('Expected Node target')
      }
      return new NodeProxy(this.context, itemTarget.id, itemTarget.nodeType)
    })
  }
  addChild = (child: NodeProxy): Promise<void> => {
    return this.context.call(this.toTarget(), 'addChild', [child.toTarget()])
  }
  removeChild = (child: NodeProxy): Promise<void> => {
    return this.context.call(this.toTarget(), 'removeChild', [child.toTarget()])
  }
  sortChildren = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'sortChildren', [])
  }
  disconnectAll = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'disconnectAll', [])
  }
  process = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'process', [])
  }
}
