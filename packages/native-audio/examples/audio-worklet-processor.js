// WASM module will be available via createAudioModule (injected by parent)
let Module
let engine

class AudioProcessor extends AudioWorkletProcessor {
  constructor() {
    super()
    this.initialized = false
    this.port.onmessage = this.handleMessage.bind(this)
  }

  async handleMessage(event) {
    const { type, data } = event.data
    try {
      switch (type) {
        case 'init':
          await this.initializeModule()
          break
        case 'setupEngine':
          this.setupEngine(data)
          break
        case 'cleanup':
          this.cleanupEngine()
          break
        case 'runTest':
          this.runTest(data)
          break
        default:
          this.port.postMessage({
            type: 'error',
            error: `Unknown message type: ${type}`,
          })
      }
    } catch (error) {
      this.port.postMessage({ type: 'error', error: error.message })
    }
  }

  async initializeModule() {
    try {
      const moduleConfig = {
        // Use pre-loaded WASM binary instead of fetching
        // eslint-disable-next-line no-undef
        wasmBinary: preloadedWasmBinary,
        locateFile: (path) => {
          if (path.endsWith('.wasm')) {
            return '../web/build/' + path
          }
          return path
        },
      }
      // eslint-disable-next-line no-undef
      Module = await createAudioModule(moduleConfig)
      const constants = Object.entries(Module)
        .sort(([keyA], [keyB]) => keyA.localeCompare(keyB))
        // eslint-disable-next-line no-unused-vars
        .filter(([_key, value]) => typeof value === 'number')
        .map(([key]) => key)
        .join(', ')
      const functions = Object.entries(Module)
        .sort(([keyA], [keyB]) => keyA.localeCompare(keyB))
        // eslint-disable-next-line no-unused-vars
        .filter(([_key, value]) => typeof value === 'function')
        .map(([key]) => key)
        .join(', ')
      this.initialized = true
      this.port.postMessage({
        type: 'initialized',
        data: { constants, functions },
      })
    } catch (error) {
      this.port.postMessage({ type: 'error', error: error.message })
    }
  }

  setupEngine({ sampleRate, numChannels, bufferSize }) {
    const nodeProcessor = new Module.NodeProcessor(0, 2, bufferSize, sampleRate)
    const phasor = new Module.Phasor()
    const osc = new Module.TableOscillator(Module.Type.RATIO)
    const filter = new Module.Biquad()
    const gain = new Module.Multiplication(Module.Type.RATIO, Module.Space.TIME)
    phasor.setNumChannels(numChannels)
    osc.setNumChannels(numChannels)
    filter.setNumChannels(numChannels)
    gain.setNumChannels(numChannels)
    const root = nodeProcessor.getDefaultNode()
    root.addChild(phasor)
    root.addChild(osc)
    root.addChild(filter)
    root.addChild(gain)
    phasor.getFrequency().setAllChannelValues(55)
    filter.getFrequency().setAllChannelValues(880)
    filter.getMode().setAllChannelValues(Module.BIQUAD_MODE_LOWPASS)
    gain.getFactor().setAllChannelValues(0.5)
    const sawtoothBufferSize = 2048
    const sawtooth = new Module.Buffer(
      Module.Type.RATIO,
      Module.Space.TIME,
      0,
      0,
      numChannels,
      sawtoothBufferSize,
    )
    const wrapper = sawtooth.getWrapper()
    for (let c = 0; c < numChannels; c++) {
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
    engine = {
      nodeProcessor,
      phasor,
      osc,
      filter,
      gain,
      sawtooth,
      audioBuffer: new Module.Data(numChannels, bufferSize),
      midiBuffer: new Module.MidiBuffer(),
      numChannels,
      bufferSize,
    }
    this.port.postMessage({
      type: 'engineSetup',
      data: { sampleRate, numChannels, bufferSize },
    })
  }

  runTest({ sampleRate, numChannels, bufferSize }) {
    const nodeProcessor = new Module.NodeProcessor(0, 2, bufferSize, sampleRate)
    const phasor = new Module.Phasor()
    const osc = new Module.TableOscillator(Module.Type.RATIO)
    const filter = new Module.Biquad()
    const gain = new Module.Multiplication(Module.Type.RATIO, Module.Space.TIME)
    phasor.setNumChannels(numChannels)
    osc.setNumChannels(numChannels)
    filter.setNumChannels(numChannels)
    gain.setNumChannels(numChannels)
    const root = nodeProcessor.getDefaultNode()
    root.addChild(phasor)
    root.addChild(osc)
    root.addChild(filter)
    root.addChild(gain)
    phasor.getFrequency().setAllChannelValues(55)
    filter.getFrequency().setAllChannelValues(880)
    filter.getMode().setAllChannelValues(Module.BIQUAD_MODE_LOWPASS)
    gain.getFactor().setAllChannelValues(0.5)
    const sawtoothBufferSize = 2048
    const sawtooth = new Module.Buffer(
      Module.Type.RATIO,
      Module.Space.TIME,
      0,
      0,
      numChannels,
      sawtoothBufferSize,
    )
    const wrapper = sawtooth.getWrapper()
    for (let c = 0; c < numChannels; c++) {
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
    const audioBuffer = new Module.Data(numChannels, bufferSize)
    const midiBuffer = new Module.MidiBuffer()
    nodeProcessor.process(audioBuffer, midiBuffer)
    const left = audioBuffer.getReadChannelData(0)
    const right = audioBuffer.getReadChannelData(1)
    let leftPeak = 0
    let rightPeak = 0
    for (let i = 0; i < bufferSize; i++) {
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
    nodeProcessor.delete()
    phasor.delete()
    osc.delete()
    filter.delete()
    gain.delete()
    sawtooth.delete()
    audioBuffer.delete()
    midiBuffer.delete()
    this.port.postMessage({
      type: 'testComplete',
      data: {
        leftPeak,
        rightPeak,
        arrayLeft,
        arrayRight,
        arrayLength,
        sampleRate,
        numChannels,
        bufferSize,
      },
    })
  }

  cleanupEngine() {
    if (engine) {
      engine.nodeProcessor.delete()
      engine.phasor.delete()
      engine.osc.delete()
      engine.filter.delete()
      engine.gain.delete()
      engine.sawtooth.delete()
      engine.audioBuffer.delete()
      engine.midiBuffer.delete()
      engine = null
    }
    this.port.postMessage({ type: 'cleanupComplete' })
  }

  process(inputs, outputs) {
    if (!this.initialized || !engine) {
      return true
    }
    const output = outputs[0]
    engine.nodeProcessor.process(engine.audioBuffer, engine.midiBuffer)
    const left = engine.audioBuffer.getReadChannelData(0)
    const right = engine.audioBuffer.getReadChannelData(1)
    for (let i = 0; i < output[0].length; i++) {
      output[0][i] = left[i]
      output[1][i] = right[i]
    }
    return true
  }
}

registerProcessor('audio-worklet-processor', AudioProcessor)
