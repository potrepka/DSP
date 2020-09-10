#include "AudioWrapper.h"

template <typename T>
dsp::AudioWrapper<T>::AudioWrapper(AudioBuffer<T> &audioBuffer)
        : data(audioBuffer.getArrayOfWritePointers())
        , numChannels(audioBuffer.getNumChannels())
        , numSamples(audioBuffer.getNumSamples()) {}

template <typename T>
size_t dsp::AudioWrapper<T>::getNumChannels() const {
    return numChannels;
}

template <typename T>
size_t dsp::AudioWrapper<T>::getNumSamples() const {
    return numSamples;
}

template <typename T>
T *dsp::AudioWrapper<T>::getChannelPointer(size_t channel) const {
    assert(channel < numChannels);
    return data[channel];
}

template <typename T>
dsp::AudioWrapper<T> dsp::AudioWrapper<T>::getSingleChannel(size_t channel) const {
    assert(channel < numChannels);
    return AudioWrapper(data + channel, 1, startSample, numSamples);
}

template <typename T>
dsp::AudioWrapper<T> dsp::AudioWrapper<T>::getSampleRange(size_t sampleOffset, size_t numSamples) const {
    assert(sampleOffset + numSamples <= this->numSamples);
    return AudioWrapper(data, numChannels, startSample + sampleOffset, numSamples);
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::clear() {
    return fill(0.0);
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::fill(T value) {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        std::fill(a, a + numSamples, value);
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::copyFrom(const AudioWrapper<T> &src) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::add(T value) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::add(AudioWrapper<T> src) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::multiplyBy(T value) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::multiplyBy(AudioWrapper<T> src) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::addProductOf(AudioWrapper<T> src, T value) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::addProductOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithNegativeOf(AudioWrapper<T> src) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithAbsoluteValueOf(AudioWrapper<T> src) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithSumOf(AudioWrapper<T> src, T value) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithSumOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithProductOf(AudioWrapper<T> src, T value) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithProductOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithMinOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
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
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithMaxOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
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
T dsp::AudioWrapper<T>::getSample(size_t channel, size_t sampleOffset) const {
    assert(channel < numChannels);
    assert(sampleOffset < numSamples);
    return data[channel][startSample + sampleOffset];
}

template <typename T>
void dsp::AudioWrapper<T>::setSample(size_t channel, size_t sampleOffset, T value) const {
    assert(channel < numChannels);
    assert(sampleOffset < numSamples);
    data[channel][startSample + sampleOffset] = value;
}

template <typename T>
dsp::AudioWrapper<T>::AudioWrapper(T *const *data, size_t numChannels, size_t numSamples)
        : data(data)
        , numChannels(numChannels)
        , numSamples(numSamples) {}

template <typename T>
dsp::AudioWrapper<T>::AudioWrapper(T *const *data, size_t numChannels, size_t startSample, size_t numSamples)
        : data(data)
        , numChannels(numChannels)
        , startSample(startSample)
        , numSamples(numSamples) {}

template class dsp::AudioWrapper<float>;
template class dsp::AudioWrapper<double>;
