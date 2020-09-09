#include "AudioData.h"

template <typename T>
dsp::AudioData<T>::AudioData(size_t numChannels, size_t numSamples) {
    setSize(numChannels, numSamples);
}

template <typename T>
size_t dsp::AudioData<T>::getNumChannels() const {
    return numChannels;
}

template <typename T>
size_t dsp::AudioData<T>::getNumSamples() const {
    return numSamples;
}

template <typename T>
void dsp::AudioData<T>::setSize(size_t numChannels, size_t numSamples) {
    assert(numChannels >= 0 && numSamples >= 0);
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
void dsp::AudioData<T>::clear() {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        std::fill(data[channel].begin(), data[channel].end(), 0.0);
    }
}

template <typename T>
const T *dsp::AudioData<T>::getReadPointer(size_t channel) const {
    assert(channel < numChannels);
    return data[channel].data();
}

template <typename T>
T *dsp::AudioData<T>::getWritePointer(size_t channel) {
    assert(channel < numChannels);
    return data[channel].data();
}

template <typename T>
T dsp::AudioData<T>::getMagnitude(int channel, int startSample, int numSamples) const {
    assert(channel < numChannels);
    assert(startSample + numSamples <= this->numSamples);
    if (numSamples == 0) {
        return 0.0;
    }
    auto start = data[channel].begin() + startSample;
    T lower = *std::min_element(start, start + numSamples);
    T upper = *std::max_element(start, start + numSamples);
    return std::max(std::max(lower, upper), std::max(-lower, -upper));
}

template <typename T>
T dsp::AudioData<T>::getRMSLevel(int channel, int startSample, int numSamples) const {
    assert(channel < numChannels);
    assert(startSample + numSamples <= this->numSamples);
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
const T **dsp::AudioData<T>::getArrayOfReadPointers() const {
    return const_cast<const T **>(pointers.data());
}

template <typename T>
T **dsp::AudioData<T>::getArrayOfWritePointers() {
    return pointers.data();
}

template <typename T>
dsp::AudioBlock<T>::AudioBlock(AudioData<T> &audioData)
        : data(audioData.getArrayOfWritePointers())
        , numChannels(audioData.getNumChannels())
        , numSamples(audioData.getNumSamples()) {}

template <typename T>
size_t dsp::AudioBlock<T>::getNumChannels() const {
    return numChannels;
}

template <typename T>
size_t dsp::AudioBlock<T>::getNumSamples() const {
    return numSamples;
}

template <typename T>
T *dsp::AudioBlock<T>::getChannelPointer(size_t channel) const {
    assert(channel < numChannels);
    return data[channel];
}

template <typename T>
dsp::AudioBlock<T> dsp::AudioBlock<T>::getSingleChannelBlock(size_t channel) const {
    assert(channel < numChannels);
    return AudioBlock(data + channel, 1, startSample, numSamples);
}

template <typename T>
dsp::AudioBlock<T> dsp::AudioBlock<T>::getSubBlock(size_t sampleOffset, size_t numSamples) const {
    assert(sampleOffset + numSamples <= this->numSamples);
    return AudioBlock(data, numChannels, startSample + sampleOffset, numSamples);
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::clear() {
    return fill(0.0);
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::fill(T value) {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        std::fill(a, a + numSamples, value);
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::copyFrom(const AudioBlock<T> &src) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        std::copy(b, b + numSamples, a);
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::add(T value) {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += value;
            ++a;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::add(AudioBlock<T> src) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += *b;
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::multiplyBy(T value) {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a *= value;
            ++a;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::multiplyBy(AudioBlock<T> src) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a *= *b;
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::addProductOf(AudioBlock<T> src, T value) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += *b * value;
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::addProductOf(AudioBlock<T> src1, AudioBlock<T> src2) {
    assert(numChannels == src1.numChannels == src2.numChannels);
    assert(numSamples == src1.numSamples == src2.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += *b * *c;
            ++a;
            ++b;
            ++c;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithNegativeOf(AudioBlock<T> src) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = -*b;
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithAbsoluteValueOf(AudioBlock<T> src) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            T value = *b;
            *a = value < 0.0 ? -value : value;
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithSumOf(AudioBlock<T> src, T value) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b + value;
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithSumOf(AudioBlock<T> src1, AudioBlock<T> src2) {
    assert(numChannels == src1.numChannels == src2.numChannels);
    assert(numSamples == src1.numSamples == src2.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b + *c;
            ++a;
            ++b;
            ++c;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithProductOf(AudioBlock<T> src, T value) {
    assert(numChannels == src.numChannels);
    assert(numSamples == src.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b * value;
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithProductOf(AudioBlock<T> src1, AudioBlock<T> src2) {
    assert(numChannels == src1.numChannels == src2.numChannels);
    assert(numSamples == src1.numSamples == src2.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b * *c;
            ++a;
            ++b;
            ++c;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithMinOf(AudioBlock<T> src1, AudioBlock<T> src2) {
    assert(numChannels == src1.numChannels == src2.numChannels);
    assert(numSamples == src1.numSamples == src2.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = std::min(*b, *c);
            ++a;
            ++b;
            ++c;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioBlock<T> &dsp::AudioBlock<T>::replaceWithMaxOf(AudioBlock<T> src1, AudioBlock<T> src2) {
    assert(numChannels == src1.numChannels == src2.numChannels);
    assert(numSamples == src1.numSamples == src2.numSamples);
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = std::max(*b, *c);
            ++a;
            ++b;
            ++c;
        }
    }
    return *this;
}

template <typename T>
T dsp::AudioBlock<T>::getSample(size_t channel, size_t sampleOffset) const {
    assert(channel < numChannels);
    assert(sampleOffset < numSamples);
    return data[channel][startSample + sampleOffset];
}

template <typename T>
void dsp::AudioBlock<T>::setSample(size_t channel, size_t sampleOffset, T value) const {
    assert(channel < numChannels);
    assert(sampleOffset < numSamples);
    data[channel][startSample + sampleOffset] = value;
}

template <typename T>
dsp::AudioBlock<T>::AudioBlock(T *const *data, size_t numChannels, size_t numSamples)
        : data(data)
        , numChannels(numChannels)
        , numSamples(numSamples) {}

template <typename T>
dsp::AudioBlock<T>::AudioBlock(T *const *data, size_t numChannels, size_t startSample, size_t numSamples)
        : data(data)
        , numChannels(numChannels)
        , startSample(startSample)
        , numSamples(numSamples) {}

template class dsp::AudioData<float>;
template class dsp::AudioData<double>;

template class dsp::AudioBlock<float>;
template class dsp::AudioBlock<double>;
