/**
 * MidiBuffer - Buffer for timestamped MIDI messages
 */

import { Platform } from '../platform';
import { Disposable } from '../core/Disposable';
import { MidiMessage } from './MidiMessage';

export class MidiBuffer extends Disposable {
  constructor(instance?: any) {
    super();
    if (instance) {
        this.instance = instance;
    } else {
        const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
        if (!Module) {
            throw new Error('Module not loaded. Call initialize() first.');
        }
        this.instance = new Module.MidiBuffer();
    }
  }

  addEvent(midiMessage: MidiMessage, sample: number): void {
    this.checkDisposed();
    // Assuming binding expects the raw C++ object reference
    this.instance.addEvent(midiMessage.getInstance(), sample);
  }

  clear(): void {
    this.checkDisposed();
    this.instance.clear();
  }
}
