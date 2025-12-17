/**
 * Transformer - Base class for nodes with both inputs and outputs
 * Examples: Biquad, OnePole, Multiplication, Division, all math nodes
 */

import { Node } from './Node';
import type { Input } from './Input';
import type { Output } from './Output';

export abstract class Transformer extends Node {
  /**
   * Get the input connection for this transformer node
   * @returns Input instance
   */
  getInput(): Input {
    this.checkDisposed();
    return this.instance.getInput();
  }

  /**
   * Get the output connection for this transformer node
   * @returns Output instance
   */
  getOutput(): Output {
    this.checkDisposed();
    return this.instance.getOutput();
  }
}
