import { nanoid } from 'nanoid/non-secure'
import { ID_LENGTH, NODE_PROCESSOR_ID } from '../../constants/proxy'
import { createObject } from '../../helpers/module'
import type {
  AudioModule,
  Data,
  Deletable,
  MidiBuffer,
  Node,
  NodeProcessor,
  ObjectType,
  Options,
  RequestMessage,
  ResponseMessage,
  SerializedValue,
  Target,
} from '../../types/module'
import { WeakValueMap } from '../util/WeakValueMap'

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

type WebAudioProcessorOptions = AudioWorkletNodeOptions & {
  processorOptions?: {
    numInputChannels: number
    numOutputChannels: number
    numSamples: number
    sampleRate: number
  }
}

class WebAudioProcessor extends AudioWorkletProcessor {
  module?: AudioModule
  nodeProcessor?: NodeProcessor
  audioBuffer?: Data
  midiBuffer?: MidiBuffer
  readonly #instances = new Map<string, Deletable>()
  readonly #properties = new WeakValueMap<string, object>()
  readonly #targets = new WeakMap<object, Target>()

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
      this.sendMessage({ message: 'state', state: 'running' })
    })
    this.port.onmessage = this.handleMessage
  }

  assertReady(): asserts this is this & {
    module: AudioModule
    nodeProcessor: NodeProcessor
    audioBuffer: Data
    midiBuffer: MidiBuffer
  } {
    if (
      this.module === undefined ||
      this.nodeProcessor === undefined ||
      this.audioBuffer === undefined ||
      this.midiBuffer === undefined
    ) {
      throw new Error('Module not initialized')
    }
  }

  process(inputs: Float32Array[][], outputs: Float32Array[][]): boolean {
    this.assertReady()
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

  private handleMessage = <T extends ObjectType>(
    event: MessageEvent<RequestMessage<T>>,
  ) => {
    const { data } = event
    switch (data.message) {
      case 'createObject': {
        const { requestId, objectType, options } = data
        this.createObject(requestId, objectType, options)
        break
      }
      case 'deleteObject': {
        const { requestId, objectId } = data
        this.deleteObject(requestId, objectId)
        break
      }
      case 'callStatic': {
        const { requestId, objectType, methodName, args } = data
        this.callStatic(requestId, objectType, methodName, args)
        break
      }
      case 'call': {
        const { requestId, target, methodName, args } = data
        this.call(requestId, target, methodName, args)
        break
      }
      case 'delete':
        this.delete()
        break
    }
  }

  private sendMessage = (msg: ResponseMessage) => {
    this.port.postMessage(msg)
  }

  private generateObjectId = (): string => nanoid(ID_LENGTH)

  private createObject = <T extends ObjectType>(
    requestId: string,
    objectType: T,
    options: Options<T> = {} as Options<T>,
  ) => {
    this.assertReady()
    try {
      const objectId = this.generateObjectId()
      const instance = createObject(this.module, objectType, options)
      const target = {
        __type: 'Target' as const,
        id: objectId,
        isChild: instance instanceof this.module.Node,
      }
      this.#instances.set(objectId, instance)
      this.#targets.set(instance, target)
      if (target.isChild) {
        this.nodeProcessor.getDefaultNode().addChild(instance as Node)
      }
      this.sendMessage({ message: 'response', requestId, result: target })
    } catch (error) {
      const errorMessage =
        error instanceof Error ? error.message : String(error)
      this.sendMessage({ message: 'response', requestId, error: errorMessage })
    }
  }

  private deleteObject = (requestId: string, objectId: string) => {
    this.assertReady()
    try {
      const instance = this.#instances.get(objectId)
      if (!instance) {
        throw new Error(`Object not found: ${objectId}`)
      }
      const target = this.#targets.get(instance)
      if (target?.isChild) {
        this.nodeProcessor.getDefaultNode().removeChild(instance as Node)
      }
      this.#instances.delete(objectId)
      instance.delete()
      this.sendMessage({ message: 'response', requestId, result: true })
    } catch (error) {
      const errorMessage =
        error instanceof Error ? error.message : String(error)
      this.sendMessage({ message: 'response', requestId, error: errorMessage })
    }
  }

  private callStatic = (
    requestId: string,
    objectType: string,
    methodName: string,
    args: SerializedValue[],
  ) => {
    this.assertReady()
    try {
      const method = (this.module as Record<string, unknown>)[objectType]
      if (typeof method !== 'function') {
        throw new Error(`Method not found: ${objectType}`)
      }
      const resolvedArgs = args.map((arg) => this.deserialize(arg))
      const result = method.apply(null, resolvedArgs)
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

  private call = (
    requestId: string,
    target: Target,
    methodName: string,
    args: SerializedValue[],
  ) => {
    this.assertReady()
    try {
      const instance = this.getTargetObject(target)
      const method = (instance as Record<string, unknown>)[methodName]
      if (typeof method !== 'function') {
        throw new Error(`Method not found: ${methodName}`)
      }
      const resolvedArgs = args.map((arg) => this.deserialize(arg))
      const result = method.apply(instance, resolvedArgs)
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

  private delete() {
    this.assertReady()
    for (const [objectId, instance] of this.#instances) {
      const target = this.#targets.get(instance)
      if (target?.isChild) {
        this.nodeProcessor.getDefaultNode().removeChild(instance as Node)
      }
      this.#instances.delete(objectId)
      instance.delete()
    }
    this.midiBuffer.delete()
    this.audioBuffer.delete()
    this.nodeProcessor.delete()
    this.sendMessage({ message: 'state', state: 'closed' })
  }

  private getTargetObject = (target: Target): object => {
    const { id } = target
    if (id === NODE_PROCESSOR_ID) {
      this.assertReady()
      return this.nodeProcessor
    }
    const instance = this.#instances.get(id) ?? this.#properties.get(id)
    if (!instance) {
      throw new Error(`Object not found: ${id}`)
    }
    return instance
  }

  private isTarget = (value: unknown): value is Target =>
    typeof value === 'object' &&
    value !== null &&
    '__type' in value &&
    value.__type === 'Target' &&
    'id' in value &&
    typeof value.id === 'string'

  private serialize = (value: unknown): SerializedValue => {
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
      const existingTarget = this.#targets.get(value)
      if (existingTarget) {
        return existingTarget
      }
      const id = this.generateObjectId()
      const target = { __type: 'Target' as const, id }
      this.#properties.set(id, value)
      this.#targets.set(value, target)
      return target
    }
    return undefined
  }

  private deserialize = (value: SerializedValue): unknown => {
    if (this.isTarget(value)) {
      return this.getTargetObject(value)
    }
    return value
  }
}

registerProcessor('dsp', WebAudioProcessor)
