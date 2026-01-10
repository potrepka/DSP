#include "Functions.h"

dsp::Sample dsp::byteToUnipolar(const int value) {
  return value == 0 ? 0.0 : (value + 1) * 0.0078125;
}

int dsp::unipolarToByte(const Sample signal) {
  return signal == 0.0 ? 0 : static_cast<int>(signal * 128) - 1;
}

dsp::Sample dsp::shortToBipolar(const int value) {
  return (value > 8192 ? value - 8191 : value - 8192) * 0.0001220703125;
}

int dsp::bipolarToShort(const Sample signal) {
  return static_cast<int>(signal * 8192) + (signal > 0 ? 8191 : 8192);
}

dsp::Sample dsp::clip(const Sample signal, const Sample min, const Sample max) {
  return max < min || signal < min ? min : signal > max ? max : signal;
}

dsp::Sample dsp::wrap(const Sample signal, const Sample max) {
  return max == 0.0 ? 0.0 : signal - floor(signal / max) * max;
}

dsp::Sample dsp::linear(const Sample x1, const Sample x2, const Sample mu) {
  return x1 + mu * (x2 - x1);
}

dsp::Sample dsp::hermite(const Sample x0, const Sample x1, const Sample x2,
                         const Sample x3, const Sample mu) {
  Sample a = 0.5 * (3.0 * (x1 - x2) - x0 + x3);
  Sample b = x2 + x2 + x0 - 0.5 * (5.0 * x1 + x3);
  Sample c = 0.5 * (x2 - x0);
  return ((a * mu + b) * mu + c) * mu + x1;
}

dsp::Sample dsp::linearClipped(Sample* data, const size_t size,
                               const Sample index, const Sample defaultValue) {
  if (size == 0) {
    return defaultValue;
  }
  size_t lastIndex = size - 1;
  Sample clippedIndex = clip(index, 0.0, static_cast<Sample>(lastIndex));
  size_t indexFloor = static_cast<size_t>(clippedIndex);
  size_t i0 = indexFloor;
  size_t i1 = indexFloor < lastIndex ? indexFloor + 1 : lastIndex;
  Sample x0 = data[i0];
  Sample x1 = data[i1];
  Sample mu = clippedIndex - static_cast<Sample>(indexFloor);
  return linear(x0, x1, mu);
}

dsp::Sample dsp::hermiteClipped(Sample* data, const size_t size,
                                const Sample index, const Sample defaultValue) {
  if (size == 0) {
    return defaultValue;
  }
  size_t lastIndex = size - 1;
  Sample clippedIndex = clip(index, 0.0, static_cast<Sample>(lastIndex));
  size_t indexFloor = static_cast<size_t>(clippedIndex);
  size_t i0 = indexFloor > 0 ? indexFloor - 1 : 0;
  size_t i1 = indexFloor;
  size_t i2 = indexFloor < lastIndex ? indexFloor + 1 : lastIndex;
  size_t i3 = indexFloor + 1 < lastIndex ? indexFloor + 2 : lastIndex;
  Sample x0 = data[i0];
  Sample x1 = data[i1];
  Sample x2 = data[i2];
  Sample x3 = data[i3];
  Sample mu = clippedIndex - static_cast<Sample>(indexFloor);
  return hermite(x0, x1, x2, x3, mu);
}

dsp::Sample dsp::linearWrapped(Sample* data, const size_t size,
                               const Sample index, const Sample defaultValue) {
  if (size == 0) {
    return defaultValue;
  }
  Sample wrappedIndex = wrap(index, static_cast<Sample>(size));
  size_t indexFloor = static_cast<size_t>(wrappedIndex);
  size_t i1 = indexFloor % size;
  size_t i2 = (indexFloor + 1) % size;
  Sample x1 = data[i1];
  Sample x2 = data[i2];
  Sample mu = wrappedIndex - static_cast<Sample>(indexFloor);
  return linear(x1, x2, mu);
}

dsp::Sample dsp::hermiteWrapped(Sample* data, const size_t size,
                                const Sample index, const Sample defaultValue) {
  if (size == 0) {
    return defaultValue;
  }
  Sample wrappedIndex = wrap(index, static_cast<Sample>(size));
  size_t indexFloor = static_cast<size_t>(wrappedIndex);
  size_t i0 = (indexFloor + size - 1) % size;
  size_t i1 = indexFloor % size;
  size_t i2 = (indexFloor + 1) % size;
  size_t i3 = (indexFloor + 2) % size;
  Sample x0 = data[i0];
  Sample x1 = data[i1];
  Sample x2 = data[i2];
  Sample x3 = data[i3];
  Sample mu = wrappedIndex - static_cast<Sample>(indexFloor);
  return hermite(x0, x1, x2, x3, mu);
}
