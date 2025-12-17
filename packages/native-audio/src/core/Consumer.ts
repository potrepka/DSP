/**
 * Consumer - Base class for nodes with only inputs (no outputs)
 * Examples: Recorder, MidiOutput
 */

import { Node } from './Node';
import type { Input } from './Input';

export abstract class Consumer extends Node {
  /**
   * Get the input connection for this consumer node
   * @returns Input instance
   */
  getInput(): Input {
    this.checkDisposed();
    return this.instance.getInput();
  }
}
