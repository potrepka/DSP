/**
 * Noise - White and pink noise generator
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Noise extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Noise();
  }

  /**
   * Get the mode input (WHITE or PINK)
   */
  getMode(): Input {
    this.checkDisposed();
    return this.instance.getMode();
  }
}
