/**
 * NormalizedFFT - FFT implementation for audio analysis
 */

import { Disposable } from './Disposable';
import { Platform } from '../platform';

export class NormalizedFFT extends Disposable {
  constructor(size: number) {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.NormalizedFFT(size);
  }

  getSize(): number {
    this.checkDisposed();
    return this.instance.getSize();
  }

  getComplexSize(): number {
      this.checkDisposed();
      return this.instance.getComplexSize();
  }

  setSize(size: number): void {
    this.checkDisposed();
    this.instance.setSize(size);
  }

  toRealImaginary(time: Float32Array, real: Float32Array, imaginary: Float32Array): void {
      this.checkDisposed();
      if (Platform.isWeb) {
          const Module = (window as any).Module;
          const timePtr = Module.HEAPF32.byteOffset + time.byteOffset;
          const realPtr = Module.HEAPF32.byteOffset + real.byteOffset;
          const imagPtr = Module.HEAPF32.byteOffset + imaginary.byteOffset;
          this.instance.toRealImaginary(timePtr, realPtr, imagPtr);
      } else {
          // Native implementation handles typed arrays directly (conceptually)
          // or we need wrappers. Assuming native binding handles it or throws.
          this.instance.toRealImaginary(time, real, imaginary);
      }
  }

  fromRealImaginary(real: Float32Array, imaginary: Float32Array, time: Float32Array): void {
      this.checkDisposed();
      if (Platform.isWeb) {
          const Module = (window as any).Module;
          const realPtr = Module.HEAPF32.byteOffset + real.byteOffset;
          const imagPtr = Module.HEAPF32.byteOffset + imaginary.byteOffset;
          const timePtr = Module.HEAPF32.byteOffset + time.byteOffset;
          this.instance.fromRealImaginary(realPtr, imagPtr, timePtr);
      } else {
          this.instance.fromRealImaginary(real, imaginary, time);
      }
  }

  toMagnitudePhase(time: Float32Array, magnitude: Float32Array, phase: Float32Array): void {
      this.checkDisposed();
      if (Platform.isWeb) {
          const Module = (window as any).Module;
          const timePtr = Module.HEAPF32.byteOffset + time.byteOffset;
          const magPtr = Module.HEAPF32.byteOffset + magnitude.byteOffset;
          const phasePtr = Module.HEAPF32.byteOffset + phase.byteOffset;
          this.instance.toMagnitudePhase(timePtr, magPtr, phasePtr);
      } else {
          this.instance.toMagnitudePhase(time, magnitude, phase);
      }
  }

  fromMagnitudePhase(magnitude: Float32Array, phase: Float32Array, time: Float32Array): void {
      this.checkDisposed();
      if (Platform.isWeb) {
          const Module = (window as any).Module;
          const magPtr = Module.HEAPF32.byteOffset + magnitude.byteOffset;
          const phasePtr = Module.HEAPF32.byteOffset + phase.byteOffset;
          const timePtr = Module.HEAPF32.byteOffset + time.byteOffset;
          this.instance.fromMagnitudePhase( magPtr, phasePtr, timePtr);
      } else {
          this.instance.fromMagnitudePhase(magnitude, phase, time);
      }
  }
}
