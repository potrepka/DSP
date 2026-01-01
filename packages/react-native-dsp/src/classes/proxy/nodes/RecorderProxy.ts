import { chainable } from '../../../helpers/proxy'
import { Target } from '../../../types/module'
import { Chainable, ProxyContext } from '../../../types/proxy'
import { BufferProxy, InputProxy } from '../core/BufferProxy'
import { ConsumerProxy } from '../core/ConsumerProxy'

export class RecorderProxy extends ConsumerProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getMode = (): Chainable<InputProxy> => this.createInput('Mode')
  getRecordingTime = (): Chainable<number> => {
    return chainable(this.call('getRecordingTime', []))
  }
  setRecordingTime = (time: number): Chainable<void> => {
    return chainable(this.call('setRecordingTime', [time]))
  }
  getRecordingBuffer = (): Chainable<BufferProxy> => {
    return chainable(
      this.call<Target>('getRecordingBuffer', []).then(
        (target) => new BufferProxy(this.context, target),
      ),
    )
  }
  getGate = (): Chainable<InputProxy> => this.createInput('Gate')
  getReset = (): Chainable<InputProxy> => this.createInput('Reset')
}
