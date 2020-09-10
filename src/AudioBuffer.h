#pragma once

#include <cassert>
#include <cmath>
#include <vector>

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

    const T *getReadPointer(size_t channel) const;
    T *getWritePointer(size_t channel);

    T getMagnitude(int channel, int startSample, int numSamples) const;
    T getRMSLevel(int channel, int startSample, int numSamples) const;

    const T **getArrayOfReadPointers() const;
    T **getArrayOfWritePointers();

private:
    size_t numChannels = 0;
    size_t numSamples = 0;
    Vector2D<T> data;
    std::vector<T *> pointers;
};

} // namespace dsp
