/**
 * Biquad - Second-order IIR filter with multiple modes
 * Supports lowpass, highpass, bandpass, notch, shelving, and peaking filters
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';

export class Biquad extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Biquad();
  }

  /**
   * Get the frequency input (Hz)
   */
  getFrequency(): Input {
    this.checkDisposed();
    return this.instance.getFrequency();
  }

  /**
   * Get the resonance input (Q factor)
   */
  getResonance(): Input {
    this.checkDisposed();
    return this.instance.getResonance();
  }

  /**
   * Get the amplitude input (gain in dB for shelving/peaking filters)
   */
  getAmplitude(): Input {
    this.checkDisposed();
    return this.instance.getAmplitude();
  }

  /**
   * Get the mode input (filter type: LOW_PASS, HIGH_PASS, etc.)
   */
  getMode(): Input {
    this.checkDisposed();
    return this.instance.getMode();
  }
}
