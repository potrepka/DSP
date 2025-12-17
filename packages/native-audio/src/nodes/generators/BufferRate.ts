/**
 * BufferRate - Outputs the buffer rate (buffer frequency)
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';

export class BufferRate extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.BufferRate();
  }
}
