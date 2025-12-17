/**
 * VariableDelay - Delay line with variable delay time
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class VariableDelay extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.VariableDelay();
  }

  getMaximumDelay(): number { this.checkDisposed(); return this.instance.getMaximumDelay(); }
  setMaximumDelay(maxDelay: number): void { this.checkDisposed(); this.instance.setMaximumDelay(maxDelay); }

  getDelay(): Input { this.checkDisposed(); return this.instance.getDelay(); }
  getFeedback(): Input { this.checkDisposed(); return this.instance.getFeedback(); }
  getInterpolation(): Input { this.checkDisposed(); return this.instance.getInterpolation(); }
  getInputSampleAndHold(): Input { this.checkDisposed(); return this.instance.getInputSampleAndHold(); }
  getOutputSampleAndHold(): Input { this.checkDisposed(); return this.instance.getOutputSampleAndHold(); }
}
