// eslint-disable-next-line @typescript-eslint/triple-slash-reference
/// <reference path="./audioWorklet.d.ts" />

import type { Data, MidiBuffer, Module, NodeProcessor } from '../../types'

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
  private nodeProcessor?: NodeProcessor
  private audioBuffer?: Data
  private midiBuffer?: MidiBuffer

  constructor(options: WebAudioProcessorOptions) {
    super()
    if (!options.processorOptions) {
      throw new Error('processorOptions is required')
    }
    const { numInputChannels, numOutputChannels, numSamples, sampleRate } =
      options.processorOptions
    createAudioModule({ wasmBinary: preloadedWasmBinary }).then((module) => {
      this.nodeProcessor = new module.NodeProcessor(
        numInputChannels,
        numOutputChannels,
        numSamples,
        sampleRate,
      )
      this.audioBuffer = new module.Data(
        Math.max(numInputChannels, numOutputChannels),
        numSamples,
      )
      this.midiBuffer = new module.MidiBuffer()
    })
  }

  process(_inputs: Float32Array[][], outputs: Float32Array[][]): boolean {
    if (!this.nodeProcessor || !this.audioBuffer || !this.midiBuffer) {
      return true
    }
    this.nodeProcessor.process(this.audioBuffer, this.midiBuffer)
    const output = outputs[0]
    for (
      let channel = 0;
      channel < this.audioBuffer.getNumChannels();
      channel++
    ) {
      const channelData = this.audioBuffer.getReadChannelData(channel)
      for (let i = 0; i < channelData.length; i++) {
        output[channel][i] = channelData[i]
      }
    }
    return true
  }

  destroy() {
    this.nodeProcessor?.delete()
    this.audioBuffer?.delete()
    this.midiBuffer?.delete()
  }
}

registerProcessor('dsp', WebAudioProcessor)
