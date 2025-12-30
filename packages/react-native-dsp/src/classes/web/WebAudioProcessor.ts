import { Space, Type } from '../../enums/global'
import {
  NodeProcessorInputName,
  NodeProcessorOutputName,
  ReservedKeyword,
} from '../../enums/module'
import { TargetType } from '../../enums/proxy'
import { constructNode, getReservedKeywords } from '../../helpers/module'
import type {
  AudioModule,
  Buffer,
  BufferOptions,
  Data,
  IncomingMessage,
  Input,
  MidiBuffer,
  Node,
  NodeOptions,
  NodeProcessor,
  NodeType,
  OutgoingMessage,
  Output,
  Target,
} from '../../types/module'

declare class AudioWorkletProcessor {
  readonly port: MessagePort
  process(
    inputs: Float32Array[][],
    outputs: Float32Array[][],
    parameters: Record<string, Float32Array>,
  ): boolean
}

declare const registerProcessor: (
  name: string,
  ctor: new (options: AudioWorkletNodeOptions) => AudioWorkletProcessor,
) => void

declare const createAudioModule: () => Promise<AudioModule>

export type WebAudioProcessorOptions = {
  processorOptions?: {
    numInputChannels: number
    numOutputChannels: number
    numSamples: number
    sampleRate: number
  }
}

class WebAudioProcessor extends AudioWorkletProcessor {
  private module?: AudioModule
  private nodeProcessor?: NodeProcessor
  private audioBuffer?: Data
  private midiBuffer?: MidiBuffer
  private buffers = new Map<string, Buffer>()
  private nodes = new Map<string, Node>()
  private targets = new WeakMap<object, Target>()

  constructor(options: WebAudioProcessorOptions) {
    super()
    const { processorOptions } = options
    if (!processorOptions) {
      throw new Error('processorOptions is required')
    }
    const { numInputChannels, numOutputChannels, numSamples, sampleRate } =
      processorOptions
    createAudioModule().then((module) => {
      this.module = module
      this.nodeProcessor = new this.module.NodeProcessor(
        numInputChannels,
        numOutputChannels,
        numSamples,
        sampleRate,
      )
      const numChannels = Math.max(numInputChannels, numOutputChannels)
      this.audioBuffer = new this.module.Data(numChannels, numSamples)
      this.midiBuffer = new this.module.MidiBuffer()
      this.sendMessage({ message: 'setState', state: 'running' })
    })
    this.port.onmessage = <T extends NodeType>(
      event: MessageEvent<IncomingMessage<T>>,
    ) => {
      const { data } = event
      this.handleMessage(data)
    }
  }

  private handleMessage<T extends NodeType>(msg: IncomingMessage<T>) {
    switch (msg.message) {
      case 'createBuffer':
        this.createBuffer(msg.bufferId, msg.options)
        break
      case 'deleteBuffer':
        this.deleteBuffer(msg.bufferId)
        break
      case 'createNode':
        this.createNode(msg.nodeId, msg.nodeType, msg.options)
        break
      case 'deleteNode':
        this.deleteNode(msg.nodeId)
        break
      case 'call':
        this.handleCall(msg.requestId, msg.target, msg.functionName, msg.args)
        break
      case 'delete':
        this.delete()
        break
    }
  }

  private sendMessage(msg: OutgoingMessage) {
    this.port.postMessage(msg)
  }

  private createBuffer(
    bufferId: string,
    options: BufferOptions = {} as BufferOptions,
  ) {
    if (!this.module) {
      throw new Error('Module not initialized')
    }
    if (this.buffers.has(bufferId)) {
      throw new Error(`Buffer already exists: ${bufferId}`)
    }
    const {
      type = Type.RATIO,
      space = Space.TIME,
      range = 0,
      defaultValue = 0,
      numChannels,
      numSamples,
      data = [],
    } = options
    const buffer = new this.module.Buffer(
      type,
      space,
      range,
      defaultValue,
      numChannels,
      numSamples,
    )
    const wrapper = buffer.getWrapper()
    for (let channel = 0; channel < numChannels; channel++) {
      const writeChannelData = wrapper.getChannelData(channel)
      for (let sample = 0; sample < numSamples; sample++) {
        writeChannelData[sample] = data[channel][sample]
      }
    }
    this.buffers.set(bufferId, buffer)
    this.registerTarget(buffer, { type: TargetType.Buffer, id: bufferId })
  }

  private deleteBuffer(bufferId: string) {
    const buffer = this.buffers.get(bufferId)
    if (!buffer) {
      throw new Error(`Buffer not found: ${bufferId}`)
    }
    this.buffers.delete(bufferId)
    buffer.delete()
  }

  private createNode<T extends NodeType>(
    nodeId: string,
    nodeType: T,
    options: NodeOptions<T> = {} as NodeOptions<T>,
  ) {
    if (!this.module || !this.nodeProcessor) {
      throw new Error('Module not initialized')
    }
    if (getReservedKeywords().includes(nodeId)) {
      throw new Error(`Keyword is reserved: ${nodeId}`)
    }
    if (this.nodes.has(nodeId)) {
      throw new Error(`Node already exists: ${nodeId}`)
    }
    const node = constructNode(this.module, nodeType, options)
    this.nodes.set(nodeId, node)
    this.registerNodeTargets(nodeId, node, nodeType)
    this.nodeProcessor.getDefaultNode().addChild(node)
  }

  private registerTarget(obj: object, target: Target) {
    this.targets.set(obj, target)
  }

  private registerNodeTargets(nodeId: string, node: Node, nodeType: NodeType) {
    this.registerTarget(node, { type: TargetType.Node, nodeType, id: nodeId })
    const inputVector = node.getInputs()
    for (let i = 0; i < inputVector.size(); i++) {
      const input = inputVector.get(i)
      const inputRecord = input as unknown as Record<string, () => string>
      this.registerTarget(input, {
        type: TargetType.Input,
        id: `${nodeId}:${inputRecord.getName()}`,
      })
    }
    const outputVector = node.getOutputs()
    for (let i = 0; i < outputVector.size(); i++) {
      const output = outputVector.get(i)
      const outputRecord = output as unknown as Record<string, () => string>
      this.registerTarget(output, {
        type: TargetType.Output,
        id: `${nodeId}:${outputRecord.getName()}`,
      })
    }
  }

  private deleteNode(nodeId: string) {
    if (!this.nodeProcessor) {
      throw new Error('Module not initialized')
    }
    const node = this.nodes.get(nodeId)
    if (!node) {
      throw new Error(`Node not found: ${nodeId}`)
    }
    this.nodeProcessor.getDefaultNode().removeChild(node)
    this.nodes.delete(nodeId)
    node.delete()
  }

  private getInput(nodeId: string, inputName: string): Input {
    if (!this.nodeProcessor) {
      throw new Error('Module not initialized')
    }
    if (nodeId === ReservedKeyword.NodeProcessor) {
      switch (inputName) {
        case NodeProcessorInputName.AudioOutput:
          return this.nodeProcessor.getAudioOutput()
        default:
          throw new Error(`Input not found: ${inputName}`)
      }
    }
    const node = this.nodes.get(nodeId)
    if (!node) {
      throw new Error(`Node not found: ${nodeId}`)
    }
    const getter = (node as unknown as Record<string, () => Input>)[
      `get${inputName}`
    ]
    if (typeof getter !== 'function') {
      throw new Error(`Input not found: ${inputName}`)
    }
    return getter.call(node)
  }

  private getOutput(nodeId: string, outputName: string): Output {
    if (!this.nodeProcessor) {
      throw new Error('Module not initialized')
    }
    if (nodeId === ReservedKeyword.NodeProcessor) {
      switch (outputName) {
        case NodeProcessorOutputName.AudioInput:
          return this.nodeProcessor.getAudioInput()
        case NodeProcessorOutputName.AudioInputClipping:
          return this.nodeProcessor.getAudioInputClipping()
        case NodeProcessorOutputName.AudioOutputClipping:
          return this.nodeProcessor.getAudioOutputClipping()
        default:
          throw new Error(`Output not found: ${outputName}`)
      }
    }
    const node = this.nodes.get(nodeId)
    if (!node) {
      throw new Error(`Node not found: ${nodeId}`)
    }
    const getter = (node as unknown as Record<string, () => Output>)[
      `get${outputName}`
    ]
    if (typeof getter !== 'function') {
      throw new Error(`Output not found: ${outputName}`)
    }
    return getter.call(node)
  }

  process(inputs: Float32Array[][], outputs: Float32Array[][]): boolean {
    if (!this.nodeProcessor || !this.audioBuffer || !this.midiBuffer) {
      return true
    }
    if (inputs.length > 0) {
      const input = inputs[0]
      for (
        let channel = 0;
        channel < this.audioBuffer.getNumChannels() && channel < input.length;
        channel++
      ) {
        const channelData = this.audioBuffer.getReadChannelData(channel)
        for (
          let sample = 0;
          sample < channelData.length && sample < input[channel].length;
          sample++
        ) {
          channelData[sample] = input[channel][sample]
        }
      }
    }
    this.nodeProcessor.process(this.audioBuffer, this.midiBuffer)
    if (outputs.length > 0) {
      const output = outputs[0]
      for (
        let channel = 0;
        channel < this.audioBuffer.getNumChannels() && channel < output.length;
        channel++
      ) {
        const channelData = this.audioBuffer.getReadChannelData(channel)
        for (
          let sample = 0;
          sample < channelData.length && sample < output[channel].length;
          sample++
        ) {
          output[channel][sample] = channelData[sample]
        }
      }
    }
    return true
  }

  delete() {
    if (!this.nodeProcessor || !this.audioBuffer || !this.midiBuffer) {
      throw new Error('Module not initialized')
    }
    for (const [nodeId, node] of this.nodes) {
      this.nodeProcessor.getDefaultNode().removeChild(node)
      this.nodes.delete(nodeId)
      node.delete()
    }
    this.midiBuffer.delete()
    this.audioBuffer.delete()
    this.nodeProcessor.delete()
    this.sendMessage({ message: 'setState', state: 'closed' })
  }

  private isTarget(value: unknown): value is Target {
    if (typeof value !== 'object' || value === null || !('type' in value)) {
      return false
    }
    const obj = value as Record<string, unknown>
    if (obj.type === TargetType.NodeProcessor) {
      return true
    }
    return 'id' in obj && typeof obj.id === 'string'
  }

  private resolveArgs(args: unknown[]): unknown[] {
    return args.map((arg) => {
      if (this.isTarget(arg)) {
        return this.getTargetObject(arg)
      }
      return arg
    })
  }

  private handleCall(
    requestId: string,
    target: Target,
    functionName: string,
    args: unknown[],
  ) {
    try {
      const obj = this.getTargetObject(target)
      const resolvedArgs = this.resolveArgs(args)
      const f = (obj as Record<string, unknown>)[functionName]
      if (typeof f !== 'function') {
        throw new Error(`Function not found: ${functionName}`)
      }
      const result = f.apply(obj, resolvedArgs)
      this.sendMessage({
        message: 'response',
        requestId,
        result: this.serializeResult(result),
      })
    } catch (error) {
      const errorMessage =
        error instanceof Error ? error.message : String(error)
      this.sendMessage({ message: 'response', requestId, error: errorMessage })
    }
  }

  private getTargetObject(target: Target): unknown {
    switch (target.type) {
      case TargetType.Buffer: {
        const buffer = this.buffers.get(target.id)
        if (!buffer) {
          throw new Error(`Buffer not found: ${target.id}`)
        }
        return buffer
      }
      case TargetType.Node: {
        const node = this.nodes.get(target.id)
        if (!node) {
          throw new Error(`Node not found: ${target.id}`)
        }
        return node
      }
      case TargetType.NodeProcessor: {
        if (!this.nodeProcessor) {
          throw new Error('Module not initialized')
        }
        return this.nodeProcessor
      }
      case TargetType.Input: {
        const [nodeId, inputName] = target.id.split(':')
        return this.getInput(nodeId, inputName)
      }
      case TargetType.Output: {
        const [nodeId, outputName] = target.id.split(':')
        return this.getOutput(nodeId, outputName)
      }
      case TargetType.Vector: {
        return this.getVector(target.id)
      }
    }
  }

  private getVector(vectorId: string): unknown {
    const parts = vectorId.split(':')
    const vectorName = parts.pop()
    const parentId = parts.join(':')
    if (vectorName === 'Nodes') {
      if (!this.nodeProcessor) {
        throw new Error('Module not initialized')
      }
      return this.nodeProcessor.getNodes()
    }
    const node = this.nodes.get(parentId)
    if (!node) {
      throw new Error(`Node not found: ${parentId}`)
    }
    if (vectorName === 'Connections') {
      const [nodeId, portName] = parentId.split(':')
      const portNode = this.nodes.get(nodeId)
      if (!portNode) {
        throw new Error(`Node not found: ${nodeId}`)
      }
      const portGetter = (portNode as unknown as Record<string, () => unknown>)[
        `get${portName}`
      ]
      if (typeof portGetter !== 'function') {
        throw new Error(`Port not found: ${portName}`)
      }
      const port = portGetter.call(portNode) as Record<string, () => unknown>
      return port.getConnections()
    }
    const getter = (node as unknown as Record<string, () => unknown>)[
      `get${vectorName}`
    ]
    if (typeof getter !== 'function') {
      throw new Error(`Vector not found: ${vectorName}`)
    }
    return getter.call(node)
  }

  private serializeResult(
    result: unknown,
  ): undefined | null | number | boolean | string | Array<number> | Target {
    if (result === undefined || result === null) {
      return result
    }
    if (
      typeof result === 'number' ||
      typeof result === 'boolean' ||
      typeof result === 'string'
    ) {
      return result
    }
    if (result instanceof Float64Array || result instanceof Float32Array) {
      return Array.from(result)
    }
    if (typeof result === 'object') {
      return this.targets.get(result)
    }
    return undefined
  }
}

registerProcessor('dsp', WebAudioProcessor)
