#pragma once

#include "AudioBuffer.h"

#include <type_traits>

#ifdef __APPLE__
#ifndef DSP_NO_VDSP
#define DSP_USE_VDSP
#endif
#endif

#ifdef DSP_USE_VDSP
#include <Accelerate/Accelerate.h>
#endif

namespace dsp {

template <typename T>
class AudioWrapper {

public:
    AudioWrapper() = default;
    AudioWrapper(AudioBuffer<T> &audioBuffer);

    size_t getNumChannels() const;
    size_t getNumSamples() const;

    T *getChannelPointer(size_t channel) const;

    AudioWrapper getSingleChannel(size_t channel) const;
    AudioWrapper getSampleRange(size_t sampleOffset, size_t numSamples) const;

    AudioWrapper &clear();
    AudioWrapper &fill(T value);

    AudioWrapper &copyFrom(const AudioWrapper<T> &src);

    AudioWrapper &add(T value);
    AudioWrapper &add(AudioWrapper<T> src);
    AudioWrapper &multiplyBy(T value);
    AudioWrapper &multiplyBy(AudioWrapper<T> src);
    AudioWrapper &addProductOf(AudioWrapper<T> src, T value);
    AudioWrapper &addProductOf(AudioWrapper<T> src1, AudioWrapper<T> src2);

    AudioWrapper &replaceWithNegativeOf(AudioWrapper<T> src);
    AudioWrapper &replaceWithAbsoluteValueOf(AudioWrapper<T> src);
    AudioWrapper &replaceWithSumOf(AudioWrapper<T> src, T value);
    AudioWrapper &replaceWithSumOf(AudioWrapper<T> src1, AudioWrapper<T> src2);
    AudioWrapper &replaceWithProductOf(AudioWrapper<T> src, T value);
    AudioWrapper &replaceWithProductOf(AudioWrapper<T> src1, AudioWrapper<T> src2);
    AudioWrapper &replaceWithMinOf(AudioWrapper<T> src1, AudioWrapper<T> src2);
    AudioWrapper &replaceWithMaxOf(AudioWrapper<T> src1, AudioWrapper<T> src2);

    T getSample(size_t channel, size_t sampleOffset) const;
    void setSample(size_t channel, size_t sampleOffset, T value) const;

private:
    T *const *data;
    size_t numChannels = 0;
    size_t numSamples = 0;
    size_t startSample = 0;

    AudioWrapper(T *const *data, size_t numChannels, size_t numSamples);
    AudioWrapper(T *const *data, size_t numChannels, size_t startSample, size_t numSamples);
};

} // namespace dsp
