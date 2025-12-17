/**
 * MidiMessage - Wrapper for MIDI messages
 */

import { Platform } from '../platform';
import { Disposable } from '../core/Disposable';

export class MidiMessage extends Disposable {
  constructor(byte0: number, byte1?: number, byte2?: number) {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }

    if (byte2 !== undefined) {
      this.instance = new Module.MidiMessage(byte0, byte1, byte2);
    } else if (byte1 !== undefined) {
      this.instance = new Module.MidiMessage(byte0, byte1);
    } else {
      this.instance = new Module.MidiMessage(byte0);
    }
  }

  // Static Factory Methods
  static noteOff(channel: number, noteNumber: number, velocity: number): MidiMessage {
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    const msg = Object.create(MidiMessage.prototype);
    msg.instance = Module.MidiMessage.noteOff(channel, noteNumber, velocity);
    return msg;
  }

  static noteOn(channel: number, noteNumber: number, velocity: number): MidiMessage {
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    const msg = Object.create(MidiMessage.prototype);
    msg.instance = Module.MidiMessage.noteOn(channel, noteNumber, velocity);
    return msg;
  }

  // Instance Methods
  isNote(): boolean { this.checkDisposed(); return this.instance.isNote(); }
  isNoteOff(): boolean { this.checkDisposed(); return this.instance.isNoteOff(); }
  isNoteOn(): boolean { this.checkDisposed(); return this.instance.isNoteOn(); }
  isAftertouch(): boolean { this.checkDisposed(); return this.instance.isAftertouch(); }
  isController(): boolean { this.checkDisposed(); return this.instance.isController(); }
  isProgramChange(): boolean { this.checkDisposed(); return this.instance.isProgramChange(); }
  isChannelPressure(): boolean { this.checkDisposed(); return this.instance.isChannelPressure(); }
  isPitchWheel(): boolean { this.checkDisposed(); return this.instance.isPitchWheel(); }
  isAllNotesOff(): boolean { this.checkDisposed(); return this.instance.isAllNotesOff(); }
  isSongPositionPointer(): boolean { this.checkDisposed(); return this.instance.isSongPositionPointer(); }
  isMidiClock(): boolean { this.checkDisposed(); return this.instance.isMidiClock(); }
  isMidiStart(): boolean { this.checkDisposed(); return this.instance.isMidiStart(); }
  isMidiContinue(): boolean { this.checkDisposed(); return this.instance.isMidiContinue(); }
  isMidiStop(): boolean { this.checkDisposed(); return this.instance.isMidiStop(); }

  getChannel(): number { this.checkDisposed(); return this.instance.getChannel(); }
  getNoteNumber(): number { this.checkDisposed(); return this.instance.getNoteNumber(); }
  getVelocity(): number { this.checkDisposed(); return this.instance.getVelocity(); }
  getAfterTouchValue(): number { this.checkDisposed(); return this.instance.getAfterTouchValue(); }
  getControllerNumber(): number { this.checkDisposed(); return this.instance.getControllerNumber(); }
  getControllerValue(): number { this.checkDisposed(); return this.instance.getControllerValue(); }
  getProgramChangeNumber(): number { this.checkDisposed(); return this.instance.getProgramChangeNumber(); }
  getChannelPressureValue(): number { this.checkDisposed(); return this.instance.getChannelPressureValue(); }
  getPitchWheelValue(): number { this.checkDisposed(); return this.instance.getPitchWheelValue(); }
  getSongPositionPointerMidiBeat(): number { this.checkDisposed(); return this.instance.getSongPositionPointerMidiBeat(); }

  getBytes(): Uint8Array {
      this.checkDisposed();
      const vec = this.instance.getBytes(); // Returns std::vector<uint8_t>
      const size = vec.size();
      const arr = new Uint8Array(size);
      for(let i=0; i<size; i++) {
          arr[i] = vec.get(i);
      }
      return arr;
  }
}
