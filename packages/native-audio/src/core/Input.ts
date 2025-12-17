/**
 * Input - Node input for connections
 */

import { Buffer } from './Buffer';
import { Output } from './Output';

export class Input extends Buffer {
  constructor(instance: any) {
      // Input is usually instantiated by C++ Node and passed here.
      // It inherits from Buffer, so we pass instance to Buffer's logic (which sets checking)
      super(instance);
  }

  // Mode enum mirroring C++ Input::Mode
  // Assuming these are exposed or we use numbers
  // static Mode = { SUM: 0, MINIMUM: 1, MAXIMUM: 2 }; // Define if needed or import

  getMode(): any { this.checkDisposed(); return this.instance.getMode(); }
  setMode(mode: any): void { this.checkDisposed(); this.instance.setMode(mode); }

  connect(output: Output): void {
    this.checkDisposed();
    // Assuming connect takes shared_ptr<Output>, wrapping it or passing naked ptr
    // The binding expects shared_ptr<Output>.
    // TS Wrapper approach for shared_ptrs usually passes the 'instance' which is the smart_ptr or object.
    this.instance.connect(output['instance']);
  }

  disconnect(output: Output): void {
    this.checkDisposed();
    this.instance.disconnect(output['instance']);
  }

  disconnectAll(): void {
    this.checkDisposed();
    this.instance.disconnectAll();
  }
}
