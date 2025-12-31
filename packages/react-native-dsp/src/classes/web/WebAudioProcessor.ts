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
  BufferVector,
  Data,
  Deletable,
  IncomingMessage,
  Input,
  InputVector,
  MidiBuffer,
  Node,
  NodeProcessor,
  NodeType,
  NodeVector,
  ObjectType,
  Options,
  OutgoingMessage,
  Output,
  OutputVector,
  SerializedValue,
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
  private objects = new Map<string, Deletable>()
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
    this.port.onmessage = <T extends ObjectType>(
      event: MessageEvent<IncomingMessage<T>>,
    ) => {
      const { data } = event
      this.handleMessage(data)
    }
  }

  private handleMessage<T extends ObjectType>(msg: IncomingMessage<T>) {
    switch (msg.message) {
      case 'createObject': {
        const { objectId, objectType, options } = msg
        this.createObject(objectId, objectType, options)
        break
      }
      case 'deleteObject': {
        const { objectId } = msg
        this.deleteObject(objectId)
        break
      }
      case 'callMethod': {
        const { requestId, target, methodName, args } = msg
        this.callMethod(requestId, target, methodName, args)
        break
      }
      case 'delete':
        this.delete()
        break
    }
  }

  private sendMessage(msg: OutgoingMessage) {
    this.port.postMessage(msg)
  }

  private createObject<T extends ObjectType>(
    objectId: string,
    objectType: T,
    options: Options<T> = {} as Options<T>,
  ) {
    if (!this.module) {
      throw new Error('Module not initialized')
    }

    // Check for reserved keywords
    if (getReservedKeywords().includes(objectId)) {
      throw new Error(`Keyword is reserved: ${objectId}`)
    }

    // Check if object already exists
    if (this.objects.has(objectId)) {
      throw new Error(`Object already exists: ${objectId}`)
    }

    // Handle Buffer creation specially (needs data handling)
    if (objectType === 'Buffer') {
      const bufferOptions = options as Options<'Buffer'>
      const {
        type = Type.RATIO,
        space = Space.TIME,
        range = 0,
        defaultValue = 0,
        numChannels = 1,
        numSamples = 1,
        data = [],
      } = bufferOptions
      const buffer = new this.module.Buffer(
        type,
        space,
        range,
        defaultValue,
        numChannels,
        numSamples,
      )
      const wrapper = buffer.getWrapper()
      for (
        let channel = 0;
        channel < numChannels && channel < data.length;
        channel++
      ) {
        const channelData = data[channel]
        const writeChannelData = wrapper.getChannelData(channel)
        for (
          let sample = 0;
          sample < numSamples && sample < channelData.length;
          sample++
        ) {
          writeChannelData[sample] = channelData[sample]
        }
      }
      this.objects.set(objectId, buffer)
      this.registerTarget(buffer, { type: TargetType.Buffer, id: objectId })
      return
    }

    // Handle Node types
    if (objectType in this.module && this.nodeProcessor) {
      const node = constructNode(
        this.module,
        objectType as NodeType,
        options as Options<NodeType>,
      )
      this.objects.set(objectId, node)
      this.registerTarget(node, {
        type: TargetType.Node,
        nodeType: objectType as NodeType,
        id: objectId,
      })
      this.nodeProcessor.getDefaultNode().addChild(node)
      return
    }

    throw new Error(`Unknown object type: ${objectType}`)
  }

  private deleteObject(objectId: string) {
    const obj = this.objects.get(objectId)
    if (!obj) {
      throw new Error(`Object not found: ${objectId}`)
    }

    // Remove from parent if it's a node
    if (this.nodeProcessor && 'delete' in obj) {
      const target = this.targets.get(obj)
      if (target && target.type === TargetType.Node) {
        this.nodeProcessor.getDefaultNode().removeChild(obj as Node)
      }
    }

    this.objects.delete(objectId)
    if ('delete' in obj && typeof obj.delete === 'function') {
      obj.delete()
    }
  }

  private registerTarget(obj: object, target: Target) {
    this.targets.set(obj, target)
  }

  private getProperty<
    T extends
      | Buffer
      | BufferVector
      | Input
      | InputVector
      | Node
      | NodeVector
      | Output
      | OutputVector,
  >(
    targetType:
      | TargetType.Buffer
      | TargetType.BufferVector
      | TargetType.Input
      | TargetType.InputVector
      | TargetType.Node
      | TargetType.NodeVector
      | TargetType.Output
      | TargetType.OutputVector,
    nodeId: string,
    propertyName: string,
    nodeType?: NodeType,
  ): T {
    if (targetType === TargetType.Node && nodeType === undefined) {
      throw new Error('nodeType is required for Node target')
    }
    const node = this.objects.get(nodeId)
    if (!node) {
      throw new Error(`Node not found: ${nodeId}`)
    }
    const getter = (node as unknown as Record<string, () => T>)[
      `get${propertyName}`
    ]
    if (typeof getter !== 'function') {
      throw new Error(`Property not found: ${propertyName}`)
    }
    const result = getter.call(node)
    const target =
      targetType === TargetType.Node
        ? {
            type: targetType,
            id: `${nodeId}:${propertyName}`,
            nodeType: nodeType!,
          }
        : {
            type: targetType,
            id: `${nodeId}:${propertyName}`,
          }
    this.registerTarget(result as object, target)
    return result
  }

  private getBuffer(nodeId: string, bufferName: string): Buffer {
    return this.getProperty(TargetType.Buffer, nodeId, bufferName)
  }

  private getBufferVector(nodeId: string, vectorName: string): BufferVector {
    return this.getProperty(TargetType.BufferVector, nodeId, vectorName)
  }

  private getInput(nodeId: string, inputName: string): Input {
    if (!this.nodeProcessor) {
      throw new Error('Module not initialized')
    }
    if (nodeId === ReservedKeyword.NodeProcessor) {
      switch (inputName) {
        case NodeProcessorInputName.AudioOutput: {
          const input = this.nodeProcessor.getAudioOutput()
          this.registerTarget(input, {
            type: TargetType.Input,
            id: `${nodeId}:${inputName}`,
          })
          return input
        }
        default:
          throw new Error(`Input not found: ${inputName}`)
      }
    }
    return this.getProperty<Input>(TargetType.Input, nodeId, inputName)
  }

  private getInputVector(nodeId: string, vectorName: string): InputVector {
    return this.getProperty(TargetType.InputVector, nodeId, vectorName)
  }

  private getNode(nodeId: string, nodeName: string, nodeType: NodeType): Node {
    return this.getProperty(TargetType.Node, nodeId, nodeName, nodeType)
  }

  private getNodeVector(nodeId: string, vectorName: string): NodeVector {
    return this.getProperty(TargetType.NodeVector, nodeId, vectorName)
  }

  private getOutput(nodeId: string, outputName: string): Output {
    if (!this.nodeProcessor) {
      throw new Error('Module not initialized')
    }
    if (nodeId === ReservedKeyword.NodeProcessor) {
      switch (outputName) {
        case NodeProcessorOutputName.AudioInput: {
          const output = this.nodeProcessor.getAudioInput()
          this.registerTarget(output, {
            type: TargetType.Output,
            id: `${nodeId}:${outputName}`,
          })
          return output
        }
        case NodeProcessorOutputName.AudioInputClipping: {
          const output = this.nodeProcessor.getAudioInputClipping()
          this.registerTarget(output, {
            type: TargetType.Output,
            id: `${nodeId}:${outputName}`,
          })
          return output
        }
        case NodeProcessorOutputName.AudioOutputClipping: {
          const output = this.nodeProcessor.getAudioOutputClipping()
          this.registerTarget(output, {
            type: TargetType.Output,
            id: `${nodeId}:${outputName}`,
          })
          return output
        }
        default:
          throw new Error(`Output not found: ${outputName}`)
      }
    }
    return this.getProperty<Output>(TargetType.Output, nodeId, outputName)
  }

  private getOutputVector(nodeId: string, vectorName: string): OutputVector {
    return this.getProperty(TargetType.OutputVector, nodeId, vectorName)
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
    for (const [objectId, object] of this.objects) {
      const target = this.targets.get(object)
      if (target && target.type === TargetType.Node) {
        this.nodeProcessor.getDefaultNode().removeChild(object as Node)
      }
      this.objects.delete(objectId)
      object.delete()
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

  private callMethod(
    requestId: string,
    target: Target,
    methodName: string,
    args: SerializedValue[],
  ) {
    try {
      const obj = this.getTargetObject(target)
      const resolvedArgs = args.map((arg) => this.deserialize(arg))
      const f = (obj as Record<string, unknown>)[methodName]
      if (typeof f !== 'function') {
        throw new Error(`Method not found: ${methodName}`)
      }
      const result = f.apply(obj, resolvedArgs)
      this.sendMessage({
        message: 'response',
        requestId,
        result: this.serialize(result),
      })
    } catch (error) {
      const errorMessage =
        error instanceof Error ? error.message : String(error)
      this.sendMessage({ message: 'response', requestId, error: errorMessage })
    }
  }

  private getTargetObject(target: Target) {
    switch (target.type) {
      case TargetType.Buffer: {
        const [nodeId, bufferName] = target.id.split(':')
        if (bufferName) {
          return this.getBuffer(nodeId, bufferName)
        }
        const buffer = this.objects.get(target.id)
        if (!buffer) {
          throw new Error(`Buffer not found: ${target.id}`)
        }
        return buffer
      }
      case TargetType.BufferVector: {
        const [nodeId, vectorName] = target.id.split(':')
        return this.getBufferVector(nodeId, vectorName)
      }
      case TargetType.Input: {
        const [nodeId, inputName] = target.id.split(':')
        return this.getInput(nodeId, inputName)
      }
      case TargetType.InputVector: {
        const [nodeId, vectorName] = target.id.split(':')
        return this.getInputVector(nodeId, vectorName)
      }
      case TargetType.Node: {
        const [nodeId, nodeName] = target.id.split(':')
        if (nodeName) {
          return this.getNode(nodeId, nodeName, target.nodeType)
        }
        const node = this.objects.get(target.id)
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
      case TargetType.NodeVector: {
        const [nodeId, vectorName] = target.id.split(':')
        return this.getNodeVector(nodeId, vectorName)
      }
      case TargetType.Output: {
        const [nodeId, outputName] = target.id.split(':')
        return this.getOutput(nodeId, outputName)
      }
      case TargetType.OutputVector: {
        const [nodeId, vectorName] = target.id.split(':')
        return this.getOutputVector(nodeId, vectorName)
      }
    }
  }

  private serialize(value: unknown): SerializedValue {
    if (value === undefined || value === null) {
      return value
    }
    if (
      typeof value === 'number' ||
      typeof value === 'boolean' ||
      typeof value === 'string'
    ) {
      return value
    }
    if (value instanceof Float64Array || value instanceof Float32Array) {
      return Array.from(value)
    }
    if (typeof value === 'object') {
      return this.targets.get(value)
    }
    return undefined
  }

  private deserialize(value: SerializedValue): unknown {
    if (this.isTarget(value)) {
      return this.getTargetObject(value)
    }
    return value
  }
}

registerProcessor('dsp', WebAudioProcessor)
