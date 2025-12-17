/**
 * Convolver - FFT Convolver
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Convolver extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Convolver();
  }

  getHeadSize(): number { this.checkDisposed(); return this.instance.getHeadSize(); }
  setHeadSize(size: number): void { this.checkDisposed(); this.instance.setHeadSize(size); }
  getTailSize(): number { this.checkDisposed(); return this.instance.getTailSize(); }
  setTailSize(size: number): void { this.checkDisposed(); this.instance.setTailSize(size); }
}
