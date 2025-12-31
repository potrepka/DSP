import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from '../core/BaseProxy'

export class MidiMessageProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  isNote = (): Chainable<boolean> => {
    return chainable(this.call('isNote', []))
  }
  isNoteOff = (): Chainable<boolean> => {
    return chainable(this.call('isNoteOff', []))
  }
  isNoteOn = (): Chainable<boolean> => {
    return chainable(this.call('isNoteOn', []))
  }
  isAftertouch = (): Chainable<boolean> => {
    return chainable(this.call('isAftertouch', []))
  }
  isController = (): Chainable<boolean> => {
    return chainable(this.call('isController', []))
  }
  isProgramChange = (): Chainable<boolean> => {
    return chainable(this.call('isProgramChange', []))
  }
  isChannelPressure = (): Chainable<boolean> => {
    return chainable(this.call('isChannelPressure', []))
  }
  isPitchWheel = (): Chainable<boolean> => {
    return chainable(this.call('isPitchWheel', []))
  }
  isAllNotesOff = (): Chainable<boolean> => {
    return chainable(this.call('isAllNotesOff', []))
  }
  isSongPositionPointer = (): Chainable<boolean> => {
    return chainable(this.call('isSongPositionPointer', []))
  }
  isMidiClock = (): Chainable<boolean> => {
    return chainable(this.call('isMidiClock', []))
  }
  isMidiStart = (): Chainable<boolean> => {
    return chainable(this.call('isMidiStart', []))
  }
  isMidiContinue = (): Chainable<boolean> => {
    return chainable(this.call('isMidiContinue', []))
  }
  isMidiStop = (): Chainable<boolean> => {
    return chainable(this.call('isMidiStop', []))
  }
  getChannel = (): Chainable<number> => {
    return chainable(this.call('getChannel', []))
  }
  getNoteNumber = (): Chainable<number> => {
    return chainable(this.call('getNoteNumber', []))
  }
  getVelocity = (): Chainable<number> => {
    return chainable(this.call('getVelocity', []))
  }
  getAftertouchValue = (): Chainable<number> => {
    return chainable(this.call('getAftertouchValue', []))
  }
  getControllerNumber = (): Chainable<number> => {
    return chainable(this.call('getControllerNumber', []))
  }
  getControllerValue = (): Chainable<number> => {
    return chainable(this.call('getControllerValue', []))
  }
  getProgramChangeNumber = (): Chainable<number> => {
    return chainable(this.call('getProgramChangeNumber', []))
  }
  getChannelPressureValue = (): Chainable<number> => {
    return chainable(this.call('getChannelPressureValue', []))
  }
  getPitchWheelValue = (): Chainable<number> => {
    return chainable(this.call('getPitchWheelValue', []))
  }
  getSongPositionPointerMidiBeat = (): Chainable<number> => {
    return chainable(this.call('getSongPositionPointerMidiBeat', []))
  }
  getBytes = (): Chainable<Uint8Array> => {
    return chainable(this.call('getBytes', []))
  }
  getRawData = (): Chainable<Uint8Array> => {
    return chainable(this.call('getRawData', []))
  }
  getRawDataSize = (): Chainable<number> => {
    return chainable(this.call('getRawDataSize', []))
  }
}
