/**
 * AudioWrapper - Provides direct access to buffer channel pointers and operations
 */

export class AudioWrapper {
  private instance: any;

  constructor(instance: any) {
    this.instance = instance;
  }

  getNumChannels(): number {
    return this.instance.getNumChannels();
  }

  getNumSamples(): number {
    return this.instance.getNumSamples();
  }

  getChannelPointer(channel: number): any {
    return this.instance.getChannelPointer(channel);
  }

  getSingleChannel(channel: number): AudioWrapper {
    return new AudioWrapper(this.instance.getSingleChannel(channel));
  }

  getSampleRange(sampleOffset: number, numSamples: number): AudioWrapper {
    return new AudioWrapper(this.instance.getSampleRange(sampleOffset, numSamples));
  }

  clear(): AudioWrapper {
    this.instance.clear();
    return this;
  }

  fill(value: number): AudioWrapper {
    this.instance.fill(value);
    return this;
  }

  copyFrom(src: AudioWrapper): AudioWrapper {
    this.instance.copyFrom(src.instance);
    return this;
  }

  add(valueOrWrapper: number | AudioWrapper): AudioWrapper {
    if (typeof valueOrWrapper === 'number') {
      this.instance.add(valueOrWrapper);
    } else {
      this.instance.addWrapper(valueOrWrapper.instance);
    }
    return this;
  }

  multiplyBy(valueOrWrapper: number | AudioWrapper): AudioWrapper {
    if (typeof valueOrWrapper === 'number') {
      this.instance.multiplyBy(valueOrWrapper);
    } else {
      this.instance.multiplyByWrapper(valueOrWrapper.instance);
    }
    return this;
  }

  addProductOf(src: AudioWrapper, valueOrWrapper: number | AudioWrapper): AudioWrapper {
    // Note: C++ overloads might need careful handling. The bindings use select_overload.
    // However, JS wrapper needs to check types.
    // The binding names are: addProductOf (src, double) and addProductOfWrappers (src, wrapper)
    if (typeof valueOrWrapper === 'number') {
        this.instance.addProductOf(src.instance, valueOrWrapper);
    } else {
        this.instance.addProductOfWrappers(src.instance, valueOrWrapper.instance);
    }
    return this;
  }

  replaceWithNegativeOf(src: AudioWrapper): AudioWrapper {
      this.instance.replaceWithNegativeOf(src.instance);
      return this;
  }

  replaceWithAbsoluteValueOf(src: AudioWrapper): AudioWrapper {
      this.instance.replaceWithAbsoluteValueOf(src.instance);
      return this;
  }

  replaceWithSumOf(src: AudioWrapper, valueOrWrapper: number | AudioWrapper): AudioWrapper {
    if (typeof valueOrWrapper === 'number') {
        this.instance.replaceWithSumOf(src.instance, valueOrWrapper);
    } else {
        this.instance.replaceWithSumOfWrappers(src.instance, valueOrWrapper.instance);
    }
    return this;
  }

  replaceWithProductOf(src: AudioWrapper, valueOrWrapper: number | AudioWrapper): AudioWrapper {
    if (typeof valueOrWrapper === 'number') {
        this.instance.replaceWithProductOf(src.instance, valueOrWrapper);
    } else {
        this.instance.replaceWithProductOfWrappers(src.instance, valueOrWrapper.instance);
    }
    return this;
  }

  replaceWithMinOf(src: AudioWrapper, valueOrWrapper: number | AudioWrapper): AudioWrapper {
     if (typeof valueOrWrapper === 'number') {
        this.instance.replaceWithMinOf(src.instance, valueOrWrapper);
    } else {
        this.instance.replaceWithMinOfWrappers(src.instance, valueOrWrapper.instance);
    }
    return this;
  }

  replaceWithMaxOf(src: AudioWrapper, valueOrWrapper: number | AudioWrapper): AudioWrapper {
     if (typeof valueOrWrapper === 'number') {
        this.instance.replaceWithMaxOf(src.instance, valueOrWrapper);
    } else {
        this.instance.replaceWithMaxOfWrappers(src.instance, valueOrWrapper.instance);
    }
    return this;
  }

  getSample(channel: number, sampleOffset: number): number {
      return this.instance.getSample(channel, sampleOffset);
  }

  setSample(channel: number, sampleOffset: number, value: number): void {
      this.instance.setSample(channel, sampleOffset, value);
  }

  /**
   * Helper to access the raw Emscripten instance
   */
  get nativeInstance(): any {
      return this.instance;
  }
}
