import { Platform } from "./platform";

export const initialize = async (): Promise<void> => {
  if (Platform.isWeb) {
    if (typeof window === 'undefined') {
      throw new Error('Window is undefined on Web platform.');
    }
    // Check if module is already loaded
    if (typeof (window as any).Module !== 'undefined') {
      return;
    }
    // Load WASM module
    try {
      const createModule = (window as any).createNativeAudioModule;
      if (!createModule) {
        throw new Error('WASM module not found. Make sure native-audio.js is loaded.');
      }
      const module = await createModule();
      (window as any).Module = module;
    } catch (error) {
      throw new Error(`Failed to initialize WASM module: ${error}`);
    }
  }
  // Native platform doesn't need initialization
}

export const isInitialized = (): boolean => {
  if (Platform.isWeb) {
    if (typeof window === 'undefined') {
      throw new Error('Window is undefined on Web platform.');
    }
    return typeof (window as any).Module !== 'undefined';
  } else if (Platform.isNative) {
    if (typeof globalThis === 'undefined') {
      throw new Error('Global is undefined on Native platform.');
    }
    return typeof (globalThis as any).NativeAudio !== 'undefined';
  }
  return false;
}
