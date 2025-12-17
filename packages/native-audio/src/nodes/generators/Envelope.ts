/**
 * Envelope - ADSR envelope generator
 * Attack-Release envelope with gate control
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';
import type { Input } from '../../core/Input';
import type { Output } from '../../core/Output';

export class Envelope extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Envelope();
  }

  /**
   * Get the attack time input (seconds)
   */
  getAttack(): Input {
    this.checkDisposed();
    return this.instance.getAttack();
  }

  /**
   * Get the release time input (seconds)
   */
  getRelease(): Input {
    this.checkDisposed();
    return this.instance.getRelease();
  }

  /**
   * Get the attack shape input (LINEAR or EXPONENTIAL)
   */
  getAttackShape(): Input {
    this.checkDisposed();
    return this.instance.getAttackShape();
  }

  /**
   * Get the release shape input (LINEAR or EXPONENTIAL)
   */
  getReleaseShape(): Input {
    this.checkDisposed();
    return this.instance.getReleaseShape();
  }

  /**
   * Get the gate input (trigger envelope on/off)
   */
  getGate(): Input {
    this.checkDisposed();
    return this.instance.getGate();
  }

  /**
   * Get the reset input (reset envelope to beginning)
   */
  getReset(): Input {
    this.checkDisposed();
    return this.instance.getReset();
  }

  /**
   * Get the current time output (for monitoring envelope state)
   */
  getCurrentTime(): Output {
    this.checkDisposed();
    return this.instance.getCurrentTime();
  }
}
