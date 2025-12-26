export type Parameter = {
  setAllChannelValues: (value: number) => void
  getChannelValue: (channel: number) => number
  setChannelValue: (channel: number, value: number) => void
}

export type Output = {
  connect: (target: Input | Parameter) => void
  disconnect: () => void
}

export type Input = {
  connect: (source: Output) => void
  disconnect: () => void
}

export type Node = {
  addChild: (child: ProcessorNode) => void
  removeChild: (child: ProcessorNode) => void
  getNumChildren: () => number
}

export type ProcessorNode = {
  setNumChannels: (count: number) => void
  getNumChannels: () => number
  delete: () => void
}

export type NodeProcessorInstance = ProcessorNode & {
  process: (audioData: DataInstance, midiData: MidiBufferInstance) => void
  getDefaultNode: () => Node
  getAudioOutput: () => Output
  getMidiOutput: () => MidiBufferInstance
}

export type PhasorInstance = ProcessorNode & {
  getFrequency: () => Parameter
  getPhase: () => Parameter
  getOutput: () => Output
}

export type TableOscillatorInstance = ProcessorNode & {
  getPhase: () => Parameter
  getTables: () => VectorBuffer
  getOutput: () => Output
}

export type BiquadInstance = ProcessorNode & {
  getInput: () => Input
  getFrequency: () => Parameter
  getResonance: () => Parameter
  getMode: () => Parameter
  getOutput: () => Output
}

export type MultiplicationInstance = ProcessorNode & {
  getInput: () => Input
  getFactor: () => Parameter
  getOutput: () => Output
}

export type BufferWrapper = {
  getSample: (channel: number, sample: number) => number
  setSample: (channel: number, sample: number, value: number) => void
}

export type BufferInstance = ProcessorNode & {
  getWrapper: () => BufferWrapper
  getNumSamples: () => number
}

export type VectorBuffer = {
  size: () => number
  get: (index: number) => BufferInstance
  push_back: (buffer: BufferInstance) => void
  clear: () => void
}

export type DataInstance = {
  getReadChannelData: (channel: number) => Float32Array
  getWriteChannelData: (channel: number) => Float32Array
  getNumChannels: () => number
  getNumSamples: () => number
  delete: () => void
}

export type MidiBufferInstance = {
  getNumMessages: () => number
  clear: () => void
  delete: () => void
}

/**
 * Emscripten Module interface for react-native-dsp WASM.
 * Represents the fully initialized WebAssembly module.
 */
export type NativeAudioModule = {
  _malloc: (size: number) => number
  _free: (ptr: number) => void
  HEAPF32: Float32Array
  HEAPU8: Uint8Array

  NodeProcessor: new (
    midiInputs: number,
    midiOutputs: number,
    bufferSize: number,
    sampleRate: number,
  ) => NodeProcessorInstance

  Phasor: new () => PhasorInstance
  TableOscillator: new (type: number) => TableOscillatorInstance
  Biquad: new () => BiquadInstance
  Multiplication: new (type: number, space: number) => MultiplicationInstance

  Buffer: new (
    type: number,
    space: number,
    minValue: number,
    maxValue: number,
    numChannels: number,
    bufferSize: number,
  ) => BufferInstance

  Data: new (numChannels: number, bufferSize: number) => DataInstance
  MidiBuffer: new () => MidiBufferInstance

  Type: {
    RATIO: number
    INTERVAL: number
  }

  Space: {
    TIME: number
    FREQUENCY: number
  }

  BIQUAD_MODE_LOWPASS: number
  BIQUAD_MODE_HIGHPASS: number
  BIQUAD_MODE_BANDPASS: number
  BIQUAD_MODE_NOTCH: number
  BIQUAD_MODE_ALLPASS: number
  BIQUAD_MODE_PEAKING: number
  BIQUAD_MODE_LOWSHELF: number
  BIQUAD_MODE_HIGHSHELF: number
}

/**
 * Configuration options for Emscripten module initialization.
 */
export type EmscriptenModuleConfig = {
  wasmBinary?: ArrayBuffer
  locateFile?: (path: string, scriptDirectory: string) => string
  onRuntimeInitialized?: () => void
  print?: (text: string) => void
  printErr?: (text: string) => void
}

/**
 * Emscripten Module factory function.
 * Returns a Promise that resolves when WASM is loaded and initialized.
 */
export type NativeAudioModuleFactory = (
  config?: Partial<EmscriptenModuleConfig>,
) => Promise<NativeAudioModule>

/**
 * Global flag to ensure worklet processor is registered only once.
 */
let workletProcessorRegistered = false

/**
 * Registers the AudioWorklet processor module with the AudioContext.
 * Uses lazy loading - WASM module is loaded when processor is first created.
 *
 * @param audioContext - The AudioContext to register the worklet processor with
 * @param processorName - Name to register the processor under (default: 'audio-graph-processor')
 * @param wasmModuleUrl - URL to the .wasmmodule.js file (default: auto-detected)
 * @param processorUrl - URL to the processor implementation (default: auto-detected)
 * @returns Promise that resolves when worklet module is registered
 *
 * @example
 * ```typescript
 * const audioContext = new AudioContext()
 * await registerAudioWorkletProcessor(audioContext)
 * ```
 */
export async function registerAudioWorkletProcessor(
  audioContext: AudioContext,
  // eslint-disable-next-line @typescript-eslint/no-unused-vars
  _processorName: string = 'audio-graph-processor',
  wasmModuleUrl?: string,
  processorUrl?: string,
): Promise<void> {
  if (workletProcessorRegistered) {
    return
  }

  const defaultWasmUrl = new URL(
    '../../web/build/react-native-dsp.js',
    import.meta.url,
  ).href
  const defaultProcessorUrl = new URL(
    '../classes/AudioGraphProcessor.js',
    import.meta.url,
  ).href

  const finalWasmUrl = wasmModuleUrl ?? defaultWasmUrl
  const finalProcessorUrl = processorUrl ?? defaultProcessorUrl

  const wasmResponse = await fetch(finalWasmUrl)
  const wasmText = await wasmResponse.text()

  const wasmBinaryUrl = finalWasmUrl.replace('.js', '.wasm')
  const wasmBinaryResponse = await fetch(wasmBinaryUrl)
  const wasmBinary = await wasmBinaryResponse.arrayBuffer()

  const processorResponse = await fetch(finalProcessorUrl)
  const processorText = await processorResponse.text()

  const wasmTextWithoutExport = wasmText
    .replace(/export\s*\{\s*\w+\s*as\s*default\s*\};?/g, '')
    .replace(/export\s+default\s+\w+;?/g, '')

  const workletCode = `
globalThis.URL = class URL {
  constructor(url, base) {
    this.href = url;
  }
};
${wasmTextWithoutExport}
globalThis.createAudioModule = createAudioModule;
globalThis.preloadedWasmBinary = new Uint8Array([${Array.from(new Uint8Array(wasmBinary)).join(',')}]).buffer;
${processorText}
  `

  const blob = new Blob([workletCode], {
    type: 'application/javascript;charset=utf-8',
  })
  const blobUrl = URL.createObjectURL(blob)

  try {
    await audioContext.audioWorklet.addModule(blobUrl)
    workletProcessorRegistered = true
  } finally {
    URL.revokeObjectURL(blobUrl)
  }
}

/**
 * Checks if the AudioWorklet processor has been registered.
 */
export function isWorkletProcessorRegistered(): boolean {
  return workletProcessorRegistered
}

/**
 * Resets the registration flag. Only for testing purposes.
 * @internal
 */
export function _resetWorkletRegistration(): void {
  workletProcessorRegistered = false
}
