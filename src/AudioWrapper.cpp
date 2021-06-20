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
    DSP_ASSERT(channel < numChannels);
    return data[channel];
}

template <typename T>
dsp::AudioWrapper<T> dsp::AudioWrapper<T>::getSingleChannel(size_t channel) const {
    DSP_ASSERT(channel < numChannels);
    return AudioWrapper(data + channel, 1, startSample, numSamples);
}

template <typename T>
dsp::AudioWrapper<T> dsp::AudioWrapper<T>::getSampleRange(size_t sampleOffset, size_t numSamples) const {
    DSP_ASSERT(sampleOffset + numSamples <= this->numSamples);
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
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vfill(&value, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vfillD(&value, a, 1, numSamples);
        }
#else
        std::fill(a, a + numSamples, value);
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::apply(std::function<T(T)> f) {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = f(*a);
            ++a;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithApplicationOf(std::function<T(T)> f, AudioWrapper<T> src) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = f(*b);
            ++a;
            ++b;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &
dsp::AudioWrapper<T>::replaceWithApplicationOf(std::function<T(T, T)> f, AudioWrapper<T> src1, AudioWrapper<T> src2) {
    size_t numChannels = std::min({this->numChannels, src1.numChannels, src2.numChannels});
    size_t numSamples = std::min({this->numSamples, src1.numSamples, src2.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = f(*b, *c);
            ++a;
            ++b;
            ++c;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithApplicationOf(std::function<T(T, T, T)> f,
                                                                     AudioWrapper<T> src1,
                                                                     AudioWrapper<T> src2,
                                                                     AudioWrapper<T> src3) {
    size_t numChannels = std::min({this->numChannels, src1.numChannels, src2.numChannels, src3.numChannels});
    size_t numSamples = std::min({this->numSamples, src1.numSamples, src2.numSamples, src3.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
        auto d = src3.data[channel] + src3.startSample;
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = f(*b, *c, *d);
            ++a;
            ++b;
            ++c;
            ++d;
        }
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::copyFrom(const AudioWrapper<T> &src) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
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
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vsadd(a, 1, &value, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vsaddD(a, 1, &value, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += value;
            ++a;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::add(AudioWrapper<T> src) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vadd(a, 1, b, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vaddD(a, 1, b, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += *b;
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::multiplyBy(T value) {
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vsmul(a, 1, &value, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vsmulD(a, 1, &value, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a *= value;
            ++a;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::multiplyBy(AudioWrapper<T> src) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vmul(a, 1, b, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vmulD(a, 1, b, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a *= *b;
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::addProductOf(AudioWrapper<T> src, T value) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vsma(b, 1, &value, a, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vsmaD(b, 1, &value, a, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += *b * value;
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::addProductOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
    size_t numChannels = std::min({this->numChannels, src1.numChannels, src2.numChannels});
    size_t numSamples = std::min({this->numSamples, src1.numSamples, src2.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vma(b, 1, c, 1, a, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vmaD(b, 1, c, 1, a, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a += *b * *c;
            ++a;
            ++b;
            ++c;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithNegativeOf(AudioWrapper<T> src) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vneg(b, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vnegD(b, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = -*b;
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithAbsoluteValueOf(AudioWrapper<T> src) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vabs(b, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vabsD(b, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            T value = *b;
            *a = value < 0.0 ? -value : value;
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithSumOf(AudioWrapper<T> src, T value) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vsadd(b, 1, &value, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vsaddD(b, 1, &value, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b + value;
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithSumOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
    size_t numChannels = std::min({this->numChannels, src1.numChannels, src2.numChannels});
    size_t numSamples = std::min({this->numSamples, src1.numSamples, src2.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vadd(b, 1, c, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vaddD(b, 1, c, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b + *c;
            ++a;
            ++b;
            ++c;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithProductOf(AudioWrapper<T> src, T value) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vsmul(b, 1, &value, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vsmulD(b, 1, &value, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b * value;
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithProductOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
    size_t numChannels = std::min({this->numChannels, src1.numChannels, src2.numChannels});
    size_t numSamples = std::min({this->numSamples, src1.numSamples, src2.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vmul(b, 1, c, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vmulD(b, 1, c, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = *b * *c;
            ++a;
            ++b;
            ++c;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithMinOf(AudioWrapper<T> src, T value) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vmin(b, 1, &value, 0, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vminD(b, 1, &value, 0, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = std::min(*b, value);
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithMinOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
    size_t numChannels = std::min({this->numChannels, src1.numChannels, src2.numChannels});
    size_t numSamples = std::min({this->numSamples, src1.numSamples, src2.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vmin(b, 1, c, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vminD(b, 1, c, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = std::min(*b, *c);
            ++a;
            ++b;
            ++c;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithMaxOf(AudioWrapper<T> src, T value) {
    size_t numChannels = std::min({this->numChannels, src.numChannels});
    size_t numSamples = std::min({this->numSamples, src.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src.data[channel] + src.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vmax(b, 1, &value, 0, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vmaxD(b, 1, &value, 0, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = std::max(*b, value);
            ++a;
            ++b;
        }
#endif
    }
    return *this;
}

template <typename T>
dsp::AudioWrapper<T> &dsp::AudioWrapper<T>::replaceWithMaxOf(AudioWrapper<T> src1, AudioWrapper<T> src2) {
    size_t numChannels = std::min({this->numChannels, src1.numChannels, src2.numChannels});
    size_t numSamples = std::min({this->numSamples, src1.numSamples, src2.numSamples});
    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto a = data[channel] + startSample;
        auto b = src1.data[channel] + src1.startSample;
        auto c = src2.data[channel] + src2.startSample;
#ifdef DSP_USE_VDSP
        if constexpr (std::is_same_v<T, float>) {
            vDSP_vmax(b, 1, c, 1, a, 1, numSamples);
        } else if constexpr (std::is_same_v<T, double>) {
            vDSP_vmaxD(b, 1, c, 1, a, 1, numSamples);
        }
#else
        for (size_t sample = 0; sample < numSamples; ++sample) {
            *a = std::max(*b, *c);
            ++a;
            ++b;
            ++c;
        }
#endif
    }
    return *this;
}

template <typename T>
T dsp::AudioWrapper<T>::getSample(size_t channel, size_t sampleOffset) const {
    DSP_ASSERT(channel < numChannels);
    DSP_ASSERT(sampleOffset < numSamples);
    return data[channel][startSample + sampleOffset];
}

template <typename T>
void dsp::AudioWrapper<T>::setSample(size_t channel, size_t sampleOffset, T value) const {
    DSP_ASSERT(channel < numChannels);
    DSP_ASSERT(sampleOffset < numSamples);
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
        , numSamples(numSamples)
        , startSample(startSample) {}

template class dsp::AudioWrapper<float>;
template class dsp::AudioWrapper<double>;
