/**
 * Producer - Base class for nodes with only outputs (no inputs)
 * Examples: Phasor, TableOscillator, Noise, Envelope, ClockTrigger
 */

import { Node } from './Node';
import type { Output } from './Output';

export abstract class Producer extends Node {
  /**
   * Get the output connection for this producer node
   * @returns Output instance
   */
  getOutput(): Output {
    this.checkDisposed();
    return this.instance.getOutput();
  }
}
