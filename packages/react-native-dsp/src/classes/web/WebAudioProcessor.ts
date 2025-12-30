import { Space, Type } from '../../enums/global'
import {
  NodeProcessorInputName,
  NodeProcessorOutputName,
  ReservedKeyword,
} from '../../enums/module'
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
  TableOscillator,
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
      case 'setInputValue':
        this.setInputValue(msg.nodeId, msg.inputName, msg.value)
        break
      case 'setInputChannelValue':
        this.setInputChannelValue(
          msg.nodeId,
          msg.inputName,
          msg.channel,
          msg.value,
        )
        break
      case 'pushTable':
        this.pushTable(msg.nodeId, msg.bufferId)
        break
      case 'connect':
        this.connect(
          msg.sourceNodeId,
          msg.sourceOutputName,
          msg.destinationNodeId,
          msg.destinationInputName,
        )
        break
      case 'disconnect':
        this.disconnect(
          msg.sourceNodeId,
          msg.sourceOutputName,
          msg.destinationNodeId,
          msg.destinationInputName,
        )
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
      range = 1,
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
    this.nodeProcessor.getDefaultNode().addChild(node)
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

  private setInputValue(nodeId: string, inputName: string, value: number) {
    const input = this.getInput(nodeId, inputName)
    input.setAllChannelValues(value)
  }

  private setInputChannelValue(
    nodeId: string,
    inputName: string,
    channel: number,
    value: number,
  ) {
    const input = this.getInput(nodeId, inputName)
    input.setChannelValue(channel, value)
  }

  private pushTable(nodeId: string, bufferId: string) {
    const node = this.nodes.get(nodeId)
    if (!node) {
      throw new Error(`Node not found: ${nodeId}`)
    }
    const buffer = this.buffers.get(bufferId)
    if (!buffer) {
      throw new Error(`Buffer not found: ${bufferId}`)
    }
    const tableOscillatorNode = node as TableOscillator
    tableOscillatorNode.getTables().push_back(buffer)
  }

  private connect(
    sourceNodeId: string,
    sourceOutputName: string,
    destinationNodeId: string,
    destinationInputName: string,
  ) {
    const output = this.getOutput(sourceNodeId, sourceOutputName)
    const input = this.getInput(destinationNodeId, destinationInputName)
    output.connect(input)
  }

  private disconnect(
    outputNodeId: string,
    outputName: string,
    inputNodeId: string,
    inputName: string,
  ) {
    const output = this.getOutput(outputNodeId, outputName)
    const input = this.getInput(inputNodeId, inputName)
    output.disconnect(input)
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
}

registerProcessor('dsp', WebAudioProcessor)
