/**
 * CompressorGate - Combined compressor and noise gate
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class CompressorGate extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.CompressorGate();
  }

  getThreshold(): Input { this.checkDisposed(); return this.instance.getThreshold(); }
  getRatio(): Input { this.checkDisposed(); return this.instance.getRatio(); }
  getAttack(): Input { this.checkDisposed(); return this.instance.getAttack(); }
  getRelease(): Input { this.checkDisposed(); return this.instance.getRelease(); }
  getMakeup(): Input { this.checkDisposed(); return this.instance.getMakeup(); }
  getSidechain(): Input { this.checkDisposed(); return this.instance.getSidechain(); }
  getMix(): Input { this.checkDisposed(); return this.instance.getMix(); }
  getGainReduction(): Input { this.checkDisposed(); return this.instance.getGainReduction(); }
}
