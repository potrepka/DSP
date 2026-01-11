#pragma once

#include <vector>

#include "Constants.h"

namespace dsp {

template <typename T>
using Vector2D = std::vector<std::vector<T>>;

template <typename T>
class AudioBuffer {
public:
  AudioBuffer(size_t numChannels, size_t numSamples);

  size_t getNumChannels() const;
  size_t getNumSamples() const;

  void setSize(size_t numChannels, size_t numSamples);
  void clear();

  const T* getReadPointer(size_t channel) const;
  T* getWritePointer(size_t channel);

  T getMagnitude(size_t channel, size_t startSample, size_t numSamples) const;
  T getRMSLevel(size_t channel, size_t startSample, size_t numSamples) const;

  const T* const* getArrayOfReadPointers() const;
  T** getArrayOfWritePointers();

private:
  size_t numChannels = 0;
  size_t numSamples = 0;
  Vector2D<T> data;
  std::vector<T*> pointers;
};

typedef AudioBuffer<Sample> Data;

} // namespace dsp
