import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { ProducerProxy } from '../core/ProducerProxy'
import { MidiBufferProxy } from '../midi/MidiBufferProxy'

export class MidiInputProxy extends ProducerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getInputMessages = (): Chainable<MidiBufferProxy> => {
    return chainable(
      this.call<Target>('getInputMessages', []).then(
        (target) => new MidiBufferProxy(this.context, target),
      ),
    )
  }
  processNote = (): Chainable<void> => {
    return chainable(this.call('processNote', []))
  }
  processNoteOn = (): Chainable<void> => {
    return chainable(this.call('processNoteOn', []))
  }
  processNoteOff = (): Chainable<void> => {
    return chainable(this.call('processNoteOff', []))
  }
  processNotePressure = (): Chainable<void> => {
    return chainable(this.call('processNotePressure', []))
  }
  processControl = (): Chainable<void> => {
    return chainable(this.call('processControl', []))
  }
  processControlValue = (): Chainable<void> => {
    return chainable(this.call('processControlValue', []))
  }
  processProgram = (): Chainable<void> => {
    return chainable(this.call('processProgram', []))
  }
  processChannelPressure = (): Chainable<void> => {
    return chainable(this.call('processChannelPressure', []))
  }
  processPitchWheel = (): Chainable<void> => {
    return chainable(this.call('processPitchWheel', []))
  }
  processAllNotesOff = (): Chainable<void> => {
    return chainable(this.call('processAllNotesOff', []))
  }
  processSongPositionInQuarterNotes = (): Chainable<void> => {
    return chainable(this.call('processSongPositionInQuarterNotes', []))
  }
  processClock = (): Chainable<void> => {
    return chainable(this.call('processClock', []))
  }
  processStart = (): Chainable<void> => {
    return chainable(this.call('processStart', []))
  }
  processContinue = (): Chainable<void> => {
    return chainable(this.call('processContinue', []))
  }
  processStop = (): Chainable<void> => {
    return chainable(this.call('processStop', []))
  }
}
