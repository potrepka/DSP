#include "AudioBuffer.h"

template <typename T>
dsp::AudioBuffer<T>::AudioBuffer(size_t numChannels, size_t numSamples) {
    setSize(numChannels, numSamples);
}

template <typename T>
size_t dsp::AudioBuffer<T>::getNumChannels() const {
    return numChannels;
}

template <typename T>
size_t dsp::AudioBuffer<T>::getNumSamples() const {
    return numSamples;
}

template <typename T>
void dsp::AudioBuffer<T>::setSize(size_t numChannels, size_t numSamples) {
    this->numChannels = numChannels;
    this->numSamples = numSamples;
    data.resize(numChannels);
    pointers.resize(numChannels);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        data[channel].resize(numSamples);
        pointers[channel] = data[channel].data();
    }
}

template <typename T>
void dsp::AudioBuffer<T>::clear() {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        std::fill(data[channel].begin(), data[channel].end(), 0.0);
    }
}

template <typename T>
const T *dsp::AudioBuffer<T>::getReadPointer(size_t channel) const {
    DSP_ASSERT(channel < numChannels);
    return data[channel].data();
}

template <typename T>
T *dsp::AudioBuffer<T>::getWritePointer(size_t channel) {
    DSP_ASSERT(channel < numChannels);
    return data[channel].data();
}

template <typename T>
T dsp::AudioBuffer<T>::getMagnitude(size_t channel, size_t startSample, size_t numSamples) const {
    DSP_ASSERT(channel < numChannels);
    DSP_ASSERT(startSample + numSamples <= this->numSamples);
    if (numSamples == 0) {
        return 0.0;
    }
    auto start = data[channel].begin() + startSample;
    T lower = *std::min_element(start, start + numSamples);
    T upper = *std::max_element(start, start + numSamples);
    return std::max(std::max(lower, upper), std::max(-lower, -upper));
}

template <typename T>
T dsp::AudioBuffer<T>::getRMSLevel(size_t channel, size_t startSample, size_t numSamples) const {
    DSP_ASSERT(channel < numChannels);
    DSP_ASSERT(startSample + numSamples <= this->numSamples);
    if (numSamples == 0) {
        return 0.0;
    }
    T sum = 0.0;
    for (size_t sample = startSample; sample < numSamples; ++sample) {
        T value = data[channel][sample];
        sum += value * value;
    }
    return sqrt(sum / numSamples);
}

template <typename T>
const T **dsp::AudioBuffer<T>::getArrayOfReadPointers() const {
    return const_cast<const T **>(pointers.data());
}

template <typename T>
T **dsp::AudioBuffer<T>::getArrayOfWritePointers() {
    return pointers.data();
}

template class dsp::AudioBuffer<float>;
template class dsp::AudioBuffer<double>;
