/**
 * Recorder - Records audio input to a buffer
 */

import { Consumer } from '../../core/Consumer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Recorder extends Consumer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Recorder();
  }

  /**
   * Get the trigger input (start/stop recording)
   */
  getTrigger(): Input {
    this.checkDisposed();
    return this.instance.getTrigger();
  }

  /**
   * Get the reset input (reset recording position)
   */
  getReset(): Input {
    this.checkDisposed();
    return this.instance.getReset();
  }
}
