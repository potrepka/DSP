export const initializeWebAudio = async (): Promise<{
  addModule: (context: BaseAudioContext) => Promise<void>
  createAudioWorkletNode: (context: BaseAudioContext) => AudioWorkletNode
}> => {
  const wasmJsUrl = new URL(
    'node_modules/@potrepka/react-native-dsp/web/build/react-native-dsp.js',
    import.meta.url,
  ).href
  const wasmWasmUrl = new URL(
    'node_modules/@potrepka/react-native-dsp/web/build/react-native-dsp.wasm',
    import.meta.url,
  ).href
  const processorUrl = new URL(
    'node_modules/@potrepka/react-native-dsp/dist/classes/web/WebAudioProcessor.js',
    import.meta.url,
  ).href
  const [jsText, wasmBinary, processorText] = await Promise.all([
    fetch(wasmJsUrl).then((r) => r.text()),
    fetch(wasmWasmUrl).then((r) => r.arrayBuffer()),
    fetch(processorUrl).then((r) => r.text()),
  ])
  const array = Array.from(new Uint8Array(wasmBinary))
  const workletCode = `
globalThis.URL = class URL { constructor(u) { this.href = u } }
${jsText}
globalThis.createAudioModule = createAudioModule
globalThis.preloadedWasmBinary = new Uint8Array([${array}]).buffer
${processorText}
`
  const addModule = async (context: BaseAudioContext) => {
    const blobUrl = URL.createObjectURL(
      new Blob([workletCode], { type: 'application/javascript' }),
    )
    try {
      await context.audioWorklet.addModule(blobUrl)
    } finally {
      URL.revokeObjectURL(blobUrl)
    }
  }
  const createAudioWorkletNode = (context: BaseAudioContext) =>
    new AudioWorkletNode(context, 'dsp')
  return {
    addModule,
    createAudioWorkletNode,
  }
}
