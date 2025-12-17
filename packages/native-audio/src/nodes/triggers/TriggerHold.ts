/**
 * TriggerHold - Holds a value when triggered
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class TriggerHold extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.TriggerHold();
  }

  getTrigger(): Input { this.checkDisposed(); return this.instance.getTrigger(); }
}
