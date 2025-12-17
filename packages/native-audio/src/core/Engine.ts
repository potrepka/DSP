/**
 * Engine - Main audio engine class
 * Manages audio I/O, processing graph, and midi processing
 */

import { Lockable } from './Lockable';
import { Platform } from '../platform';
import { NodeProcessor } from './NodeProcessor';
import { MidiProcessor } from '../midi/MidiProcessor';
import { Buffer } from './Buffer';

export class Engine extends Lockable {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.Engine();
  }

  getInputDevices(): number[] {
    this.checkDisposed();
    const vec = this.instance.getInputDevices();
    const result = [];
    for (let i = 0; i < vec.size(); i++) {
        result.push(vec.get(i));
    }
    vec.delete();
    return result;
  }

  getOutputDevices(): number[] {
    this.checkDisposed();
    const vec = this.instance.getOutputDevices();
    const result = [];
    for (let i = 0; i < vec.size(); i++) {
        result.push(vec.get(i));
    }
    vec.delete();
    return result;
  }

  getSampleRates(inputDevice: number, outputDevice: number): number[] {
    this.checkDisposed();
    const vec = this.instance.getSampleRates(inputDevice, outputDevice);
    const result = [];
    for (let i = 0; i < vec.size(); i++) {
        result.push(vec.get(i));
    }
    vec.delete();
    return result;
  }

  getDefaultInputDevice(): number {
    this.checkDisposed();
    return this.instance.getDefaultInputDevice();
  }

  getDefaultOutputDevice(): number {
    this.checkDisposed();
    return this.instance.getDefaultOutputDevice();
  }

  getDefaultSampleRate(inputDevice: number, outputDevice: number): number {
    this.checkDisposed();
    return this.instance.getDefaultSampleRate(inputDevice, outputDevice);
  }

  setup(inputDevice: number, outputDevice: number, numSamples: number, sampleRate: number): void {
    this.checkDisposed();
    this.instance.setup(inputDevice, outputDevice, numSamples, sampleRate);
  }

  start(): void {
    this.checkDisposed();
    this.instance.start();
  }

  getDeviceName(device: number): string {
    this.checkDisposed();
    return this.instance.getDeviceName(device);
  }

  getInputDeviceName(): string {
    this.checkDisposed();
    return this.instance.getInputDeviceName();
  }

  getOutputDeviceName(): string {
    this.checkDisposed();
    return this.instance.getOutputDeviceName();
  }

  getNumInputChannels(): number {
    this.checkDisposed();
    return this.instance.getNumInputChannels();
  }

  getNumOutputChannels(): number {
    this.checkDisposed();
    return this.instance.getNumOutputChannels();
  }

  getNumSamples(): number {
    this.checkDisposed();
    return this.instance.getNumSamples();
  }

  getSampleRate(): number {
    this.checkDisposed();
    return this.instance.getSampleRate();
  }

  getAudioBuffer(): Buffer {
    this.checkDisposed();
    return new Buffer(this.instance.getAudioBuffer());
  }

  getNodeProcessor(): NodeProcessor {
    this.checkDisposed();
    // Wrap the returned shared_ptr<NodeProcessor>
    return new NodeProcessor(this.instance.getNodeProcessor());
  }

  getMidiProcessor(): MidiProcessor {
    this.checkDisposed();
    return new MidiProcessor(this.instance.getMidiProcessor());
  }
}

