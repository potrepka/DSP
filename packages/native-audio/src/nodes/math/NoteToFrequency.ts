/**
 * NoteToFrequency - Converts MIDI note number to frequency
 */

import { Transformer } from '../../core/Transformer';
import { Platform } from '../../platform';

export class NoteToFrequency extends Transformer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.NoteToFrequency();
  }
}
