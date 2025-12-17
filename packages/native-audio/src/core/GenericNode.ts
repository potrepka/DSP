/**
 * GenericNode - A concrete wrapper for generic C++ Node instances
 * Used when a method returns a base Node pointer
 */

import { Node } from './Node';

export class GenericNode extends Node {
  constructor(instance: any) {
    super(instance);
  }
}
