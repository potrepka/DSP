/**
 * Multiplication - Multiply input signal with another signal or constant
 * Control multiplication by connecting signals to its input
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';

export class Multiplication extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Multiplication();
  }

  // Note: Multiplication has no additional node-specific methods beyond inherited ones
  // All control is through connecting signals to its input and output
}
