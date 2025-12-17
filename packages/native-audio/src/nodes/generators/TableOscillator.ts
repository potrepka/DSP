/**
 * TableOscillator - Wavetable synthesis oscillator
 */

import { Disposable } from '../../core/Disposable';
import { Platform } from '../../platform';

export class TableOscillator extends Disposable {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.TableOscillator();
  }

  /**
   * Get the tables input (for wavetable selection)
   */
  getTables(): any {
    this.checkDisposed();
    return this.instance.getTables();
  }

  /**
   * Get the phase input
   */
  getPhase(): any {
    this.checkDisposed();
    return this.instance.getPhase();
  }

  /**
   * Get the position input (for wavetable scanning)
   */
  getPosition(): any {
    this.checkDisposed();
    return this.instance.getPosition();
  }

  /**
   * Get the phase interpolation input
   */
  getPhaseInterpolation(): any {
    this.checkDisposed();
    return this.instance.getPhaseInterpolation();
  }

  /**
   * Get the position interpolation input
   */
  getPositionInterpolation(): any {
    this.checkDisposed();
    return this.instance.getPositionInterpolation();
  }

  /**
   * Set number of channels
   */
  setNumChannels(numChannels: number): void {
    this.checkDisposed();
    this.instance.setNumChannels(numChannels);
  }

  /**
   * Get number of channels
   */
  getNumChannels(): number {
    this.checkDisposed();
    return this.instance.getNumChannels();
  }

  /**
   * Set sample rate
   */
  setSampleRate(sampleRate: number): void {
    this.checkDisposed();
    this.instance.setSampleRate(sampleRate);
  }

  /**
   * Get sample rate
   */
  getSampleRate(): number {
    this.checkDisposed();
    return this.instance.getSampleRate();
  }

  /**
   * Set number of samples
   */
  setNumSamples(numSamples: number): void {
    this.checkDisposed();
    this.instance.setNumSamples(numSamples);
  }

  /**
   * Get number of samples
   */
  getNumSamples(): number {
    this.checkDisposed();
    return this.instance.getNumSamples();
  }

  /**
   * Get inputs
   */
  getInputs(): any {
    this.checkDisposed();
    return this.instance.getInputs();
  }

  /**
   * Get outputs
   */
  getOutputs(): any {
    this.checkDisposed();
    return this.instance.getOutputs();
  }
}
