/**
 * Buffer - Multi-channel audio buffer
 */

import { Disposable } from './Disposable';
import { Platform } from '../platform';
import { AudioWrapper } from './AudioWrapper';
import { Type, Space } from '../enums'; // Assuming these enums exist and are exported

export class Buffer extends Disposable {
  constructor(instance?: any) {
    super();
    if (instance) {
      this.instance = instance;
    } else {
      const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
      if (!Module) {
        throw new Error('Module not loaded. Call initialize() first.');
      }
      this.instance = new Module.Buffer();
    }
  }

  getType(): Type { this.checkDisposed(); return this.instance.getType(); }

  setType(type: Type): void { this.checkDisposed(); this.instance.setType(type); }

  getSpace(): Space { this.checkDisposed(); return this.instance.getSpace(); }

  setSpace(space: Space): void { this.checkDisposed(); this.instance.setSpace(space); }

  getRange(): number { this.checkDisposed(); return this.instance.getRange(); }

  setRange(range: number): void { this.checkDisposed(); this.instance.setRange(range); }

  getDefaultValue(): number { this.checkDisposed(); return this.instance.getDefaultValue(); }

  setDefaultValue(defaultValue: number): void { this.checkDisposed(); this.instance.setDefaultValue(defaultValue); }

  getNumChannels(): number {
    this.checkDisposed();
    return this.instance.getNumChannels();
  }

  setNumChannels(numChannels: number): void {
    this.checkDisposed();
    this.instance.setNumChannels(numChannels);
  }

  getNumSamples(): number {
    this.checkDisposed();
    return this.instance.getNumSamples();
  }

  setNumSamples(numSamples: number): void {
    this.checkDisposed();
    this.instance.setNumSamples(numSamples);
  }

  setSize(numChannels: number, numSamples: number): void {
      this.checkDisposed();
      this.instance.setSize(numChannels, numSamples);
  }

  getChannelValue(channel: number): number {
      this.checkDisposed();
      return this.instance.getChannelValue(channel);
  }

  setSingleChannelValue(channel: number, value: number): void {
      this.checkDisposed();
      this.instance.setSingleChannelValue(channel, value);
  }

  setAllChannelValues(value: number): void {
      this.checkDisposed();
      this.instance.setAllChannelValues(value);
  }

  getPeak(): any {
      this.checkDisposed();
      return this.instance.getPeak();
  }

  getRMS(): any {
       this.checkDisposed();
       return this.instance.getRMS();
  }

  getWrapper(): AudioWrapper {
    this.checkDisposed();
    return new AudioWrapper(this.instance.getWrapper());
  }
}
