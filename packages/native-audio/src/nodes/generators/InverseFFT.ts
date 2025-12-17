/**
 * InverseFFT - Inverse Fast Fourier Transform
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class InverseFFT extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.InverseFFT();
  }

  getMagnitude(): Input { this.checkDisposed(); return this.instance.getMagnitude(); }
  getPhase(): Input { this.checkDisposed(); return this.instance.getPhase(); }
}
