/**
 * Output - Node output for connections
 */

import { Buffer } from './Buffer';
import { Input } from './Input';

export class Output extends Buffer {
  constructor(instance: any) {
      super(instance);
  }

  connect(input: Input): void {
    this.checkDisposed();
    this.instance.connect(input['instance']);
  }

  disconnect(input: Input): void {
    this.checkDisposed();
    this.instance.disconnect(input['instance']);
  }

  disconnectAll(): void {
    this.checkDisposed();
    this.instance.disconnectAll();
  }
}
