import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BaseProxy } from '../core/BaseProxy'
import { MidiMessageProxy } from './MidiMessageProxy'

export class MidiBufferProxy extends BaseProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  begin = (): Chainable<unknown> => {
    return chainable(this.call('begin', []))
  }
  end = (): Chainable<unknown> => {
    return chainable(this.call('end', []))
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
