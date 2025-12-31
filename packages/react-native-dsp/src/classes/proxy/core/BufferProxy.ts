import { InputMode, Space, TargetType, Type } from '../../../enums'
import type { Data, ProxyContext, Target, Wrapper } from '../../../types'
import { VectorProxy } from './VectorProxy'

export class BufferProxy {
  constructor(
    protected readonly context: ProxyContext,
    public readonly id: string,
  ) {}
  toTarget = (): Target => ({ type: TargetType.Buffer, id: this.id })
  delete = async (): Promise<void> => {
    this.context.port.postMessage({
      message: 'deleteBuffer',
      bufferId: this.id,
    })
  }
  getType = (): Promise<Type> => {
    return this.context.call(this.toTarget(), 'getType', [])
  }
  setType = (type: Type): Promise<void> => {
    return this.context.call(this.toTarget(), 'setType', [type])
  }
  getSpace = (): Promise<Space> => {
    return this.context.call(this.toTarget(), 'getSpace', [])
  }
  setSpace = (space: Space): Promise<void> => {
    return this.context.call(this.toTarget(), 'setSpace', [space])
  }
  getRange = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getRange', [])
  }
  setRange = (range: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setRange', [range])
  }
  getDefaultValue = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getDefaultValue', [])
  }
  setDefaultValue = (value: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setDefaultValue', [value])
  }
  getNumChannels = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getNumChannels', [])
  }
  setNumChannels = (numChannels: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setNumChannels', [numChannels])
  }
  getNumSamples = (): Promise<number> => {
    return this.context.call(this.toTarget(), 'getNumSamples', [])
  }
  setNumSamples = (numSamples: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setNumSamples', [numSamples])
  }
  setSize = (numChannels: number, numSamples: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setSize', [
      numChannels,
      numSamples,
    ])
  }
  getChannelValues = async (): Promise<Float64Array> => {
    const result = await this.context.call<number[]>(
      this.toTarget(),
      'getChannelValues',
      [],
    )
    return new Float64Array(result)
  }
  setChannelValues = (values: Float64Array): Promise<void> => {
    return this.context.call(this.toTarget(), 'setChannelValues', [
      Array.from(values),
    ])
  }
  getChannelValue = (channel: number): Promise<number> => {
    return this.context.call(this.toTarget(), 'getChannelValue', [channel])
  }
  setChannelValue = (channel: number, value: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setChannelValue', [
      channel,
      value,
    ])
  }
  setAllChannelValues = (value: number): Promise<void> => {
    return this.context.call(this.toTarget(), 'setAllChannelValues', [value])
  }
  getPeak = async (): Promise<Float64Array> => {
    const result = await this.context.call<number[]>(
      this.toTarget(),
      'getPeak',
      [],
    )
    return new Float64Array(result)
  }
  getRMS = async (): Promise<Float64Array> => {
    const result = await this.context.call<number[]>(
      this.toTarget(),
      'getRMS',
      [],
    )
    return new Float64Array(result)
  }
  getData = (): Data => {
    throw new Error('getData returns a complex object that cannot be proxied')
  }
  getWrapper = (): Wrapper => {
    throw new Error(
      'getWrapper returns a complex object that cannot be proxied',
    )
  }
}

export class InputProxy extends BufferProxy {
  constructor(
    context: ProxyContext,
    public readonly nodeId: string,
    public readonly inputName: string,
  ) {
    super(context, `${nodeId}:${inputName}`)
  }
  override toTarget = (): Target => ({ type: TargetType.Input, id: this.id })
  getMode = (): Promise<InputMode> => {
    return this.context.call(this.toTarget(), 'getMode', [])
  }
  setMode = (mode: InputMode): Promise<void> => {
    return this.context.call(this.toTarget(), 'setMode', [mode])
  }
  getConnections = (): VectorProxy<OutputProxy> => {
    const target: Target = {
      type: TargetType.OutputVector,
      id: `${this.id}:Connections`,
    }
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      if (itemTarget.type !== TargetType.Output) {
        throw new Error('Expected Output target')
      }
      const [nodeId, outputName] = itemTarget.id.split(':')
      return new OutputProxy(this.context, nodeId, outputName)
    })
  }
  connect = (output: OutputProxy): Promise<void> => {
    return this.context.call(this.toTarget(), 'connect', [output.toTarget()])
  }
  disconnect = (output: OutputProxy): Promise<void> => {
    return this.context.call(this.toTarget(), 'disconnect', [output.toTarget()])
  }
  disconnectAll = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'disconnectAll', [])
  }
  prepareNoLock = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'prepareNoLock', [])
  }
  processNoLock = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'processNoLock', [])
  }
}

export class OutputProxy extends BufferProxy {
  constructor(
    context: ProxyContext,
    public readonly nodeId: string,
    public readonly outputName: string,
  ) {
    super(context, `${nodeId}:${outputName}`)
  }
  override toTarget = (): Target => ({ type: TargetType.Output, id: this.id })
  getConnections = (): VectorProxy<InputProxy> => {
    const target: Target = {
      type: TargetType.InputVector,
      id: `${this.id}:Connections`,
    }
    return new VectorProxy(this.context, target, (itemTarget: Target) => {
      if (itemTarget.type !== TargetType.Input) {
        throw new Error('Expected Input target')
      }
      const [nodeId, inputName] = itemTarget.id.split(':')
      return new InputProxy(this.context, nodeId, inputName)
    })
  }
  connect = (input: InputProxy): Promise<void> => {
    return this.context.call(this.toTarget(), 'connect', [input.toTarget()])
  }
  disconnect = (input: InputProxy): Promise<void> => {
    return this.context.call(this.toTarget(), 'disconnect', [input.toTarget()])
  }
  disconnectAll = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'disconnectAll', [])
  }
  prepareNoLock = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'prepareNoLock', [])
  }
  processNoLock = (): Promise<void> => {
    return this.context.call(this.toTarget(), 'processNoLock', [])
  }
}
