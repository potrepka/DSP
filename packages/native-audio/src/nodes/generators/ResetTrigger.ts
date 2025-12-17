/**
 * ResetTrigger - Generates a trigger on reset
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';

export class ResetTrigger extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.ResetTrigger();
  }
}
