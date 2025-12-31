import { chainable } from '../../../helpers/proxy'
import { MidiEvent, Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from '../core/BaseProxy'
import { MidiMessageProxy } from './MidiMessageProxy'

export class MidiBufferProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getEvents = (): Chainable<MidiEvent[]> => {
    return chainable(this.call('getEvents', []))
  }
  addEvent = (
    midiMessage: MidiMessageProxy,
    sample: number,
  ): Chainable<void> => {
    return chainable(this.call('addEvent', [midiMessage, sample]))
  }
  addEvents = (
    midiBuffer: MidiBufferProxy,
    startSample: number,
    numSamples: number,
    sampleDeltaToAdd: number,
  ): Chainable<void> => {
    return chainable(
      this.call('addEvents', [
        midiBuffer,
        startSample,
        numSamples,
        sampleDeltaToAdd,
      ]),
    )
  }
  clear = (): Chainable<void> => {
    return chainable(this.call('clear', []))
  }
}
