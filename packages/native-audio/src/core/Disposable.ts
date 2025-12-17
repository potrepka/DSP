/**
 * Base class for objects that wrap C++ instances requiring manual disposal
 */

import { Platform } from '../platform';

export abstract class Disposable {
  protected instance: any = null;
  private _disposed = false;

  /**
   * Check if this object has been disposed
   */
  isDisposed(): boolean {
    return this._disposed;
  }

  /**
   * Throw an error if this object has been disposed
   */
  protected checkDisposed(): void {
    if (this._disposed) {
      throw new Error(`Cannot use disposed ${this.constructor.name}`);
    }
  }

  /**
   * Dispose of the underlying C++ instance
   * - Web: Calls delete() to free WASM memory
   * - Native: Clears reference for GC
   */
  dispose(): void {
    if (this._disposed) {
      return;
    }

    if (Platform.isWeb && this.instance && typeof this.instance.delete === 'function') {
      this.instance.delete();
    }

    this.instance = null;
    this._disposed = true;
  }

  /**
   * Get the underlying C++ instance (for advanced usage)
   */
  getInstance(): any {
    this.checkDisposed();
    return this.instance;
  }
}
