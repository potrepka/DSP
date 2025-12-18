import { Platform } from './platform'

export const initialize = async (): Promise<void> => {
  if (Platform.isWeb) {
    if (typeof window === 'undefined') {
      throw new Error('Window is undefined on Web platform.')
    }
    // Check if module is already loaded
    // @ts-ignore
    if (typeof window.AudioModule !== 'undefined') {
      return
    }
    // Load WASM module
    try {
      // @ts-ignore
      const createModule = window.createAudioModule
      if (!createModule) {
        throw new Error(
          'WASM module not found. Make sure native-audio.js is loaded.',
        )
      }
      const module = await createModule()
      // @ts-ignore
      window.AudioModule = module
    } catch (error) {
      throw new Error(`Failed to initialize WASM module: ${error}`)
    }
  }
  // Native platform doesn't need initialization
}

export const isInitialized = (): boolean => {
  if (Platform.isWeb) {
    if (typeof window === 'undefined') {
      throw new Error('Window is undefined on Web platform.')
    }
    // @ts-ignore
    return typeof window.AudioModule !== 'undefined'
  } else if (Platform.isNative) {
    if (typeof globalThis === 'undefined') {
      throw new Error('Global is undefined on Native platform.')
    }
    // @ts-ignore
    return typeof globalThis.AudioModule !== 'undefined'
  }
  return false
}
