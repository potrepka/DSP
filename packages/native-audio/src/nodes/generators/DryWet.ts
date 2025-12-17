/**
 * DryWet - Mixes two signals
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class DryWet extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.DryWet();
  }

  getDry(): Input { this.checkDisposed(); return this.instance.getDry(); }
  getWet(): Input { this.checkDisposed(); return this.instance.getWet(); }
  getMix(): Input { this.checkDisposed(); return this.instance.getMix(); }
}
