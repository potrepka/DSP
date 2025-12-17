/**
 * Gain Node (Multiplication)
 * Simple gain/volume control
 */

import { Node } from '../../core/Node';
import { Platform } from '../../platform';
import type { AudioBuffer } from '../../types';

declare const Module: any;
declare const global: any;

/**
 * Gain node for volume control
 */
export class Gain extends Node {
  private numChannels: number;
  private sampleRate: number;

  /**
   * Create a new Gain node
   * @param numChannels - Number of audio channels (1=mono, 2=stereo, etc.)
   * @param sampleRate - Sample rate in Hz (e.g., 44100)
   * @param initialGain - Initial gain value (default: 1.0 = unity gain)
   */
  constructor(
    numChannels: number,
    sampleRate: number,
    initialGain: number = 1.0
  ) {
    super();

    this.numChannels = numChannels;
    this.sampleRate = sampleRate;

    // Create platform-specific instance
    if (Platform.isWeb) {
      if (typeof Module === 'undefined' || !Module.Multiplication) {
        throw new Error('WASM module not loaded. Make sure to load the module first.');
      }
      this.instance = new Module.Multiplication(numChannels, sampleRate);
      this.instance.setGain(initialGain);
    } else if (Platform.isNative) {
      if (typeof global.NativeAudio === 'undefined' || !global.NativeAudio.Multiplication) {
        throw new Error('Native module not installed. Make sure to install the native module first.');
      }
      this.instance = new global.NativeAudio.Multiplication(numChannels, sampleRate);
      this.instance.setGain(initialGain);
    } else {
      throw new Error('Unsupported platform');
    }
  }

  /**
   * Set the gain value
   * @param gain - Gain multiplier (1.0 = unity, 0.5 = -6dB, 2.0 = +6dB, etc.)
   */
  setGain(gain: number): void {
    this.checkDisposed();
    this.instance.setGain(gain);
  }

  /**
   * Set the gain in decibels
   * @param dB - Gain in decibels
   */
  setGainDB(dB: number): void {
    const gain = Math.pow(10, dB / 20);
    this.setGain(gain);
  }

  /**
   * Process audio through the gain node
   * @param inputs - Input audio buffers (one Float32Array per channel)
   * @param outputs - Output audio buffers (one Float32Array per channel)
   * @param bufferSize - Number of samples per channel
   */
  process(
    inputs: AudioBuffer,
    outputs: AudioBuffer,
    bufferSize: number
  ): void {
    this.checkDisposed();

    if (Platform.isWeb) {
      Module.AudioProcessor.process(this.instance, inputs, outputs, bufferSize);
    } else {
      this.instance.process(inputs, outputs, bufferSize);
    }
  }
}
