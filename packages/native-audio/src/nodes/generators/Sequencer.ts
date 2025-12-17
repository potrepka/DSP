/**
 * Sequencer - Step sequencer
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Sequencer extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Sequencer();
  }

  getTrigger(): Input { this.checkDisposed(); return this.instance.getTrigger(); }
  getReset(): Input { this.checkDisposed(); return this.instance.getReset(); }
}
