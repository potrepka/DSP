/**
 * Trigonometric - Trigonometric functions (sin, cos, tan, etc)
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Trigonometric extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Trigonometric();
  }

  getMode(): Input { this.checkDisposed(); return this.instance.getMode(); }
}
