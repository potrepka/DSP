/**
 * Phasor - Phase accumulator/ramp generator
 * Generates a linear ramp from 0 to 1 at a specified frequency
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Phasor extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Phasor();
  }

  /**
   * Get the frequency input (Hz)
   */
  getFrequency(): Input {
    this.checkDisposed();
    return this.instance.getFrequency();
  }

  /**
   * Get the mode input (WRAPPED or UNBOUNDED)
   */
  getMode(): Input {
    this.checkDisposed();
    return this.instance.getMode();
  }

  /**
   * Get the reset input (trigger to reset phase to 0)
   */
  getReset(): Input {
    this.checkDisposed();
    return this.instance.getReset();
  }
}
