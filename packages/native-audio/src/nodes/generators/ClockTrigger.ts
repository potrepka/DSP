/**
 * ClockTrigger - Pulse generator
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class ClockTrigger extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.ClockTrigger();
  }

  getInterval(): Input { this.checkDisposed(); return this.instance.getInterval(); }
  getDelayTime(): Input { this.checkDisposed(); return this.instance.getDelayTime(); }
  getReset(): Input { this.checkDisposed(); return this.instance.getReset(); }
  getCurrentTime(): Input { this.checkDisposed(); return this.instance.getCurrentTime(); }
}
