/**
 * Lockable - Base class for thread-safe objects
 */

import { Disposable } from './Disposable';

export abstract class Lockable extends Disposable {
  /**
   * Acquire the lock
   */
  lock(): void {
    this.checkDisposed();
    this.instance.lock();
  }

  /**
   * Release the lock
   */
  unlock(): void {
    this.checkDisposed();
    this.instance.unlock();
  }
}
