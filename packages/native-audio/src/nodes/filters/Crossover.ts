/**
 * Crossover - Audio crossover filter
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Crossover extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Crossover();
  }

  getFrequency(): Input {
    this.checkDisposed();
    return this.instance.getFrequency();
  }

  getResonance(): Input {
    this.checkDisposed();
    return this.instance.getResonance();
  }

  getLowpass(): Input {
    this.checkDisposed();
    return this.instance.getLowpass();
  }

  getHighpass(): Input {
    this.checkDisposed();
    return this.instance.getHighpass();
  }
}
