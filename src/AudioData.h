#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>

namespace dsp {

template <typename T>
using Vector2D = std::vector<std::vector<T>>;

template <typename T>
class AudioData {

public:
    AudioData(size_t numChannels, size_t numSamples);

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

template <typename T>
class AudioBlock {

public:
    AudioBlock() = default;
    AudioBlock(AudioData<T> &audioData);

    size_t getNumChannels() const;
    size_t getNumSamples() const;

    T *getChannelPointer(size_t channel) const;

    AudioBlock getSingleChannelBlock(size_t channel) const;
    AudioBlock getSubBlock(size_t sampleOffset, size_t numSamples) const;

    AudioBlock &clear();
    AudioBlock &fill(T value);

    AudioBlock &copyFrom(const AudioBlock<T> &src);

    AudioBlock &add(T value);
    AudioBlock &add(AudioBlock<T> src);
    AudioBlock &multiplyBy(T value);
    AudioBlock &multiplyBy(AudioBlock<T> src);
    AudioBlock &addProductOf(AudioBlock<T> src, T value);
    AudioBlock &addProductOf(AudioBlock<T> src1, AudioBlock<T> src2);

    AudioBlock &replaceWithNegativeOf(AudioBlock<T> src);
    AudioBlock &replaceWithAbsoluteValueOf(AudioBlock<T> src);
    AudioBlock &replaceWithSumOf(AudioBlock<T> src, T value);
    AudioBlock &replaceWithSumOf(AudioBlock<T> src1, AudioBlock<T> src2);
    AudioBlock &replaceWithProductOf(AudioBlock<T> src, T value);
    AudioBlock &replaceWithProductOf(AudioBlock<T> src1, AudioBlock<T> src2);
    AudioBlock &replaceWithMinOf(AudioBlock<T> src1, AudioBlock<T> src2);
    AudioBlock &replaceWithMaxOf(AudioBlock<T> src1, AudioBlock<T> src2);

    T getSample(size_t channel, size_t sampleOffset) const;
    void setSample(size_t channel, size_t sampleOffset, T value) const;

private:
    T *const *data;
    size_t numChannels = 0;
    size_t numSamples = 0;
    size_t startSample = 0;

    AudioBlock(T *const *data, size_t numChannels, size_t numSamples);
    AudioBlock(T *const *data, size_t numChannels, size_t startSample, size_t numSamples);
};

} // namespace dsp
