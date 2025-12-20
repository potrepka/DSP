import type {
  BiquadInstance,
  BufferInstance,
  DataInstance,
  MidiBufferInstance,
  MultiplicationInstance,
  NativeAudioModule,
  NativeAudioModuleFactory,
  NodeProcessorInstance,
  PhasorInstance,
  TableOscillatorInstance,
} from '../helpers/module'

declare const globalThis: {
  createAudioModule?: NativeAudioModuleFactory
  preloadedWasmBinary?: ArrayBuffer
}
declare const registerProcessor: (
  name: string,
  processor: typeof AudioGraphProcessor,
) => void

type EngineConfig = {
  sampleRate: number
  numChannels: number
  bufferSize: number
}

type Engine = {
  nodeProcessor: NodeProcessorInstance
  phasor: PhasorInstance
  osc: TableOscillatorInstance
  filter: BiquadInstance
  gain: MultiplicationInstance
  sawtooth: BufferInstance
  audioBuffer: DataInstance
  midiBuffer: MidiBufferInstance
  numChannels: number
  bufferSize: number
}

class AudioGraphProcessor extends AudioWorkletProcessor {
  private module?: NativeAudioModule
  private engine?: Engine
  private initialized = false

  constructor() {
    super()
    const createAudioModule = globalThis.createAudioModule
    if (!createAudioModule) {
      throw new Error(
        'createAudioModule not found. Did you call registerAudioWorkletProcessor()?',
      )
    }
    createAudioModule({
      wasmBinary: globalThis.preloadedWasmBinary,
    })
      .then((module) => {
        this.module = module
        this.initialized = true
        const constants = Object.entries(this.module)
          // eslint-disable-next-line @typescript-eslint/no-unused-vars
          .filter(([_key, value]) => typeof value === 'number')
          .map(([key]) => key)
          .sort()
          .join(', ')
        const functions = Object.entries(this.module)
          // eslint-disable-next-line @typescript-eslint/no-unused-vars
          .filter(([_key, value]) => typeof value === 'function')
          .map(([key]) => key)
          .sort()
          .join(', ')
        this.port.postMessage({
          type: 'initialized',
          data: { constants, functions },
        })
      })
      .catch((error) => {
        this.port.postMessage({
          type: 'error',
          error: `WASM initialization failed: ${error.message}`,
        })
      })
    this.port.onmessage = this.handleMessage.bind(this)
  }

  private createAudioGraph = (config: EngineConfig): Engine => {
    if (!this.module) {
      throw new Error('Module not initialized')
    }
    const nodeProcessor = new this.module.NodeProcessor(
      0,
      2,
      config.bufferSize,
      config.sampleRate,
    )
    const phasor = new this.module.Phasor()
    const osc = new this.module.TableOscillator(this.module.Type.RATIO)
    const filter = new this.module.Biquad()
    const gain = new this.module.Multiplication(
      this.module.Type.RATIO,
      this.module.Space.TIME,
    )
    phasor.setNumChannels(config.numChannels)
    osc.setNumChannels(config.numChannels)
    filter.setNumChannels(config.numChannels)
    gain.setNumChannels(config.numChannels)
    const root = nodeProcessor.getDefaultNode()
    root.addChild(phasor)
    root.addChild(osc)
    root.addChild(filter)
    root.addChild(gain)
    phasor.getFrequency().setAllChannelValues(55)
    filter.getFrequency().setAllChannelValues(880)
    filter.getMode().setAllChannelValues(this.module.BIQUAD_MODE_LOWPASS)
    gain.getFactor().setAllChannelValues(0.5)
    const sawtoothBufferSize = 2048
    const sawtooth = new this.module.Buffer(
      this.module.Type.RATIO,
      this.module.Space.TIME,
      0,
      0,
      config.numChannels,
      sawtoothBufferSize,
    )
    const wrapper = sawtooth.getWrapper()
    for (let c = 0; c < config.numChannels; c++) {
      for (let s = 0; s < sawtoothBufferSize; s++) {
        const phase = s / sawtoothBufferSize
        const value = 2 * ((phase + 0.5) % 1) - 1
        wrapper.setSample(c, s, value)
      }
    }
    osc.getTables().push_back(sawtooth)
    phasor.getOutput().connect(osc.getPhase())
    osc.getOutput().connect(filter.getInput())
    filter.getOutput().connect(gain.getInput())
    gain.getOutput().connect(nodeProcessor.getAudioOutput())
    const audioBuffer = new this.module.Data(
      config.numChannels,
      config.bufferSize,
    )
    const midiBuffer = new this.module.MidiBuffer()
    return {
      nodeProcessor,
      phasor,
      osc,
      filter,
      gain,
      sawtooth,
      audioBuffer,
      midiBuffer,
      numChannels: config.numChannels,
      bufferSize: config.bufferSize,
    }
  }

  private handleRunTest = (config: EngineConfig): void => {
    if (!this.module) {
      this.port.postMessage({
        type: 'error',
        error: 'Module not initialized',
      })
      return
    }
    try {
      const tempEngine = this.createAudioGraph(config)
      tempEngine.nodeProcessor.process(
        tempEngine.audioBuffer,
        tempEngine.midiBuffer,
      )
      const left = tempEngine.audioBuffer.getReadChannelData(0)
      const right = tempEngine.audioBuffer.getReadChannelData(1)
      let leftPeak = 0
      let rightPeak = 0
      for (let i = 0; i < config.bufferSize; i++) {
        const leftAmplitude = Math.abs(left[i])
        const rightAmplitude = Math.abs(right[i])
        if (leftAmplitude > leftPeak) {
          leftPeak = leftAmplitude
        }
        if (rightAmplitude > rightPeak) {
          rightPeak = rightAmplitude
        }
      }
      const arrayLength = 8
      const precision = 4
      const arrayLeft = []
      for (let i = 0; i < arrayLength; i++) {
        arrayLeft.push(left[i].toFixed(precision))
      }
      const arrayRight = []
      for (let i = 0; i < arrayLength; i++) {
        arrayRight.push(right[i].toFixed(precision))
      }
      tempEngine.nodeProcessor.delete()
      tempEngine.phasor.delete()
      tempEngine.osc.delete()
      tempEngine.filter.delete()
      tempEngine.gain.delete()
      tempEngine.sawtooth.delete()
      tempEngine.audioBuffer.delete()
      tempEngine.midiBuffer.delete()
      this.port.postMessage({
        type: 'testComplete',
        data: {
          leftPeak,
          rightPeak,
          arrayLeft,
          arrayRight,
          arrayLength,
          sampleRate: config.sampleRate,
          numChannels: config.numChannels,
          bufferSize: config.bufferSize,
        },
      })
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
    } catch (error: any) {
      this.port.postMessage({
        type: 'error',
        error: `Test failed: ${error.message}`,
      })
    }
  }

  private handleSetupEngine = (config: EngineConfig): void => {
    if (!this.module) {
      this.port.postMessage({
        type: 'error',
        error: 'Module not initialized',
      })
      return
    }
    try {
      if (this.engine) {
        this.cleanupEngine()
      }
      this.engine = this.createAudioGraph(config)
      this.port.postMessage({
        type: 'engineSetup',
        data: {
          sampleRate: config.sampleRate,
          numChannels: config.numChannels,
          bufferSize: config.bufferSize,
        },
      })
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
    } catch (error: any) {
      this.port.postMessage({
        type: 'error',
        error: `Engine setup failed: ${error.message}`,
      })
    }
  }

  private handleMessage = (event: MessageEvent): void => {
    const { type, data } = event.data
    switch (type) {
      case 'init':
        break
      case 'runTest':
        this.handleRunTest(data)
        break
      case 'setupEngine':
        this.handleSetupEngine(data)
        break
      case 'getModule':
        if (this.module) {
          this.port.postMessage({
            data: {
              constants: Object.entries(this.module)
                // eslint-disable-next-line @typescript-eslint/no-unused-vars
                .filter(([_key, value]) => typeof value === 'number')
                .map(([key]) => key)
                .sort(),
              functions: Object.entries(this.module)
                .sort(([keyA], [keyB]) => keyA.localeCompare(keyB))
                // eslint-disable-next-line @typescript-eslint/no-unused-vars
                .filter(([_key, value]) => typeof value === 'function')
                .map(([key]) => key)
                .sort(),
            },
            type: 'moduleInfo',
          })
        }
        break
      case 'cleanup':
        this.cleanupEngine()
        this.port.postMessage({ type: 'cleanupComplete' })
        break
      default:
        this.port.postMessage({
          error: `Unknown message type: ${type}`,
          type: 'error',
        })
    }
  }

  process(
    inputs: Float32Array[][],
    outputs: Float32Array[][],
    // eslint-disable-next-line @typescript-eslint/no-unused-vars
    _parameters: Record<string, Float32Array>,
  ): boolean {
    if (!this.initialized || !this.engine) {
      return true
    }
    const output = outputs[0]
    if (!output || output.length === 0) {
      return true
    }
    this.engine.nodeProcessor.process(
      this.engine.audioBuffer,
      this.engine.midiBuffer,
    )
    const leftChannel = this.engine.audioBuffer.getReadChannelData(0)
    const rightChannel = this.engine.audioBuffer.getReadChannelData(1)
    for (let i = 0; i < output[0].length; i++) {
      output[0][i] = leftChannel[i]
      if (output[1]) {
        output[1][i] = rightChannel[i]
      }
    }
    return true
  }

  private cleanupEngine = (): void => {
    if (this.engine) {
      this.engine.nodeProcessor.delete()
      this.engine.phasor.delete()
      this.engine.osc.delete()
      this.engine.filter.delete()
      this.engine.gain.delete()
      this.engine.sawtooth.delete()
      this.engine.audioBuffer.delete()
      this.engine.midiBuffer.delete()
      this.engine = undefined
    }
  }
}

registerProcessor('audio-graph-processor', AudioGraphProcessor)
