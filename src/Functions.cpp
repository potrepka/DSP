#include "Functions.h"

DSP_FLOAT dsp::negative(const DSP_FLOAT value) {
    return -value;
}

DSP_FLOAT dsp::oneOver(const DSP_FLOAT value) {
    return 1.0 / value;
}

DSP_FLOAT dsp::toBinary(const DSP_FLOAT value) {
    return value == 0.0 ? 0.0 : 1.0;
}

DSP_FLOAT dsp::toInteger(const DSP_FLOAT value) {
    return floor(value);
}

DSP_FLOAT dsp::bipolarToUnipolar(const DSP_FLOAT bipolar) {
    return 0.5 * bipolar + 0.5;
}

DSP_FLOAT dsp::unipolarToBipolar(const DSP_FLOAT unipolar) {
    return 2.0 * unipolar - 1.0;
}

DSP_FLOAT dsp::decibelsToLinear(const DSP_FLOAT decibels) {
    return ONE_OVER_SIX_DB * decibels;
}

DSP_FLOAT dsp::linearToDecibels(const DSP_FLOAT linear) {
    return SIX_DB * linear;
}

DSP_FLOAT dsp::decibelsToRatio(const DSP_FLOAT decibels) {
    return exp2(decibelsToLinear(decibels));
}

DSP_FLOAT dsp::ratioToDecibels(const DSP_FLOAT ratio) {
    return linearToDecibels(log2(ratio));
}

DSP_FLOAT dsp::clip(const DSP_FLOAT signal, const DSP_FLOAT min, const DSP_FLOAT max) {
    if (min >= max) {
        return min;
    }
    return signal < min ? min : signal > max ? max : signal;
}

DSP_FLOAT dsp::wrap(const DSP_FLOAT signal, const DSP_FLOAT min, const DSP_FLOAT max) {
    if (min == max) {
        return 0.0;
    }
    const DSP_FLOAT diff = max - min;
    const DSP_FLOAT adjusted = signal - min;
    return signal - floor(adjusted / diff) * diff;
}

DSP_FLOAT dsp::linear(const std::vector<DSP_FLOAT> &table, const DSP_FLOAT index, DSP_FLOAT defaultValue) {
    assert(index >= 0.0);
    if (table.size() == 0) {
        return defaultValue;
    }
    int indexFloor = static_cast<int>(index);
    DSP_FLOAT mu = index - indexFloor;
    DSP_FLOAT x1 = table[indexFloor % table.size()];
    DSP_FLOAT x2 = table[(indexFloor + 1) % table.size()];
    return x1 + mu * (x2 - x1);
}

DSP_FLOAT dsp::hermite(const std::vector<DSP_FLOAT> &table, const DSP_FLOAT index, DSP_FLOAT defaultValue) {
    assert(index >= 0.0);
    if (table.size() == 0) {
        return defaultValue;
    }
    int indexFloor = static_cast<int>(index);
    DSP_FLOAT mu = index - indexFloor;
    DSP_FLOAT x0 = table[(indexFloor + table.size() - 1) % table.size()];
    DSP_FLOAT x1 = table[indexFloor % table.size()];
    DSP_FLOAT x2 = table[(indexFloor + 1) % table.size()];
    DSP_FLOAT x3 = table[(indexFloor + 2) % table.size()];
    DSP_FLOAT a = 0.5 * (3.0 * (x1 - x2) - x0 + x3);
    DSP_FLOAT b = x2 + x2 + x0 - 0.5 * (5.0 * x1 + x3);
    DSP_FLOAT c = 0.5 * (x2 - x0);
    return ((a * mu + b) * mu + c) * mu + x1;
}

void dsp::ScaledFFT::setup(std::size_t size) {
    this->size = size;
    oneOverSize = 1.0 / size;

    audioFFT.init(getSize());
    time.resize(getSize());
    real.resize(getComplexSize());
    imaginary.resize(getComplexSize());
}

std::size_t dsp::ScaledFFT::getSize() {
    return size;
}

std::size_t dsp::ScaledFFT::getComplexSize() {
    return audiofft::AudioFFT::ComplexSize(size);
}

void dsp::ScaledFFT::fft(std::vector<DSP_FLOAT> &timeBuffer,
                         std::vector<DSP_FLOAT> &realBuffer,
                         std::vector<DSP_FLOAT> &imaginaryBuffer) {
    std::transform(timeBuffer.begin(), timeBuffer.end(), time.begin(), [this](DSP_FLOAT x) { return x * oneOverSize; });
    audioFFT.fft(time.data(), real.data(), imaginary.data());
    std::copy(real.begin(), real.end(), realBuffer.begin());
    std::copy(imaginary.begin(), imaginary.end(), imaginaryBuffer.begin());
}

void dsp::ScaledFFT::ifft(std::vector<DSP_FLOAT> &timeBuffer,
                          std::vector<DSP_FLOAT> &realBuffer,
                          std::vector<DSP_FLOAT> &imaginaryBuffer) {
    std::copy(realBuffer.begin(), realBuffer.end(), real.begin());
    std::copy(imaginaryBuffer.begin(), imaginaryBuffer.end(), imaginary.begin());
    audioFFT.ifft(time.data(), real.data(), imaginary.data());
    std::transform(time.begin(), time.end(), timeBuffer.begin(), [this](DSP_FLOAT x) { return x * size; });
}
