export const initializeWebAudio = async (
  audioContext: AudioContext,
): Promise<AudioWorkletNode> => {
  const wasmJsUrl = new URL(
    '../../web/build/react-native-dsp.js',
    import.meta.url,
  ).href
  const wasmWasmUrl = new URL(
    '../../web/build/react-native-dsp.wasm',
    import.meta.url,
  ).href
  const processorUrl = new URL(
    '../../dist/classes/web/WebAudioProcessor.js',
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
  const blobUrl = URL.createObjectURL(
    new Blob([workletCode], { type: 'application/javascript' }),
  )
  try {
    await audioContext.audioWorklet.addModule(blobUrl)
  } finally {
    URL.revokeObjectURL(blobUrl)
  }
  return new AudioWorkletNode(audioContext, 'dsp')
}
