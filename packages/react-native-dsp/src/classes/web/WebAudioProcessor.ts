// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="./audioWorklet.d.ts" />

import {
  NodeProcessorInputName,
  NodeProcessorOutputName,
  ReservedKeyword,
  Space,
  Type,
} from '../../enums'
import { constructNode, getReservedKeywords } from '../../helpers'
import type {
  Buffer,
  Data,
  IncomingMessage,
  Input,
  MidiBuffer,
  Module,
  Node,
  NodeProcessor,
  NodeProps,
  NodeType,
  OutgoingMessage,
  Output,
} from '../../types'

declare const createAudioModule: (opts?: {
  wasmBinary?: ArrayBuffer
}) => Promise<Module>
declare const preloadedWasmBinary: ArrayBuffer

export type WebAudioProcessorOptions = {
  processorOptions?: {
    numInputChannels: number
    numOutputChannels: number
    numSamples: number
    sampleRate: number
  }
}

class WebAudioProcessor extends AudioWorkletProcessor {
  private module?: Module
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
    createAudioModule({ wasmBinary: preloadedWasmBinary }).then((module) => {
      this.module = module
      this.nodeProcessor = new module.NodeProcessor(
        numInputChannels,
        numOutputChannels,
        numSamples,
        sampleRate,
      )
      const numChannels = Math.max(numInputChannels, numOutputChannels)
      this.audioBuffer = new module.Data(numChannels, numSamples)
      this.midiBuffer = new module.MidiBuffer()
      this.sendMessage({ messageType: 'setState', state: 'running' })
    })
    this.port.onmessage = <T extends NodeType>(
      event: MessageEvent<IncomingMessage<T>>,
    ) => {
      const { data } = event
      this.handleMessage(data)
    }
  }

  private handleMessage<T extends NodeType>(msg: IncomingMessage<T>) {
    switch (msg.messageType) {
      case 'createBuffer':
        this.createBuffer(
          msg.id,
          msg.type,
          msg.space,
          msg.range,
          msg.defaultValue,
          msg.numChannels,
          msg.numSamples,
        )
        break
      case 'destroyBuffer':
        this.destroyBuffer(msg.id)
        break
      case 'createNode':
        this.createNode(msg.id, msg.nodeType, msg.props)
        break
      case 'destroyNode':
        this.destroyNode(msg.id)
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
    }
  }

  private sendMessage(msg: OutgoingMessage) {
    this.port.postMessage(msg)
  }

  private createBuffer(
    bufferId: string,
    type: Type = Type.RATIO,
    space: Space = Space.TIME,
    range: number = 0,
    defaultValue: number = 0,
    numChannels: number,
    numSamples: number,
  ) {
    if (!this.module) {
      throw new Error('Module not initialized')
    }
    if (this.buffers.has(bufferId)) {
      throw new Error(`Buffer already exists: ${bufferId}`)
    }
    const buffer = new this.module.Buffer(
      type,
      space,
      range,
      defaultValue,
      numChannels,
      numSamples,
    )
    this.buffers.set(bufferId, buffer)
  }

  private destroyBuffer(bufferId: string) {
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
    props: NodeProps<T> = {} as NodeProps<T>,
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
    const node = constructNode(this.module, nodeType, props)
    this.nodes.set(nodeId, node)
    this.nodeProcessor.getDefaultNode().addChild(node)
  }

  private destroyNode(nodeId: string) {
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
    const node = this.nodes.get(nodeId)
    if (!node) {
      throw new Error(`Node not found: ${nodeId}`)
    }
    if (nodeId === ReservedKeyword.NodeProcessor) {
      switch (inputName) {
        case NodeProcessorInputName.AudioInput:
          return this.nodeProcessor.getAudioInput()
        default:
          throw new Error(`Input not found: ${inputName}`)
      }
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
    const node = this.nodes.get(nodeId)
    if (!node) {
      throw new Error(`Node not found: ${nodeId}`)
    }
    if (nodeId === ReservedKeyword.NodeProcessor) {
      switch (outputName) {
        case NodeProcessorOutputName.AudioOutput:
          return this.nodeProcessor.getAudioOutput()
        case NodeProcessorOutputName.AudioInputClipping:
          return this.nodeProcessor.getAudioInputClipping()
        case NodeProcessorOutputName.AudioOutputClipping:
          return this.nodeProcessor.getAudioOutputClipping()
        default:
          throw new Error(`Output not found: ${outputName}`)
      }
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

  private connect(
    outputNodeId: string,
    outputName: string,
    inputNodeId: string,
    inputName: string,
  ) {
    const output = this.getOutput(outputNodeId, outputName)
    const input = this.getInput(inputNodeId, inputName)
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
      return false
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

  destroy() {
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
    this.sendMessage({ messageType: 'setState', state: 'closed' })
  }
}

registerProcessor('dsp', WebAudioProcessor)
