/**
 * NodeProcessor - Manages the audio graph and processing
 */

import { Disposable } from './Disposable';
import { Platform } from '../platform';
import type { AudioBuffer } from '../types';

export class NodeProcessor extends Disposable {
  constructor(
    numInputChannelsOrInstance: number | any,
    numOutputChannels?: number,
    numSamples?: number,
    sampleRate?: number
  ) {
    super();
    if (typeof numInputChannelsOrInstance === 'object') {
       this.instance = numInputChannelsOrInstance;
    } else {
      const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
      if (!Module) {
        throw new Error('Module not loaded. Call initialize() first.');
      }
      this.instance = new Module.NodeProcessor(
        numInputChannelsOrInstance,
        numOutputChannels,
        numSamples,
        sampleRate
      );
    }
  }

  /**
   * Process audio through the node graph
   */
  process(inputs: AudioBuffer, outputs: AudioBuffer, bufferSize: number): void {
    this.checkDisposed();

    if (Platform.isWeb) {
      const Module = (window as any).Module;
      // Convert Float32Arrays to pointers
      const inputPtrs: number[] = [];
      const outputPtrs: number[] = [];

      for (const arr of inputs) {
        inputPtrs.push(Module.HEAPF32.byteOffset + arr.byteOffset);
      }

      for (const arr of outputs) {
        outputPtrs.push(Module.HEAPF32.byteOffset + arr.byteOffset);
      }

      this.instance.process(inputPtrs, outputPtrs, bufferSize);
    } else {
      this.instance.process(inputs, outputs, bufferSize);
    }
  }

  /**
   * Get the default root node of the processor
   */
  getDefaultNode(): any {
    this.checkDisposed();
    return this.instance.getDefaultNode();
  }

  /**
   * Get all nodes managed by the processor
   */
  getNodes(): any[] {
     this.checkDisposed();
     const vec = this.instance.getNodes();
     const result = [];
     for (let i = 0; i < vec.size(); i++) {
         result.push(vec.get(i));
     }
     vec.delete();
     return result;
  }

  // Expose other NodeProcessor methods
  isActive(): boolean { this.checkDisposed(); return this.instance.isActive(); }
  setActive(active: boolean): void { this.checkDisposed(); this.instance.setActive(active); }

  getNumInputChannels(): number { this.checkDisposed(); return this.instance.getNumInputChannels(); }
  setNumInputChannels(n: number): void { this.checkDisposed(); this.instance.setNumInputChannels(n); }

  getNumOutputChannels(): number { this.checkDisposed(); return this.instance.getNumOutputChannels(); }
  setNumOutputChannels(n: number): void { this.checkDisposed(); this.instance.setNumOutputChannels(n); }

  getSampleRate(): number { this.checkDisposed(); return this.instance.getSampleRate(); }
  setSampleRate(rate: number): void { this.checkDisposed(); this.instance.setSampleRate(rate); }
}
