/**
 * ChannelMerger - Merges multiple mono inputs into a multi-channel signal
 */

import { Producer } from '../../core/Producer';
import { Platform } from '../../platform';

export class ChannelMerger extends Producer {
  constructor() {
    super();
    const Module = Platform.isWeb ? (window as any).Module : (globalThis as any).NativeAudio;
    if (!Module) {
      throw new Error('Module not loaded. Call initialize() first.');
    }
    this.instance = new Module.ChannelMerger();
  }
}
