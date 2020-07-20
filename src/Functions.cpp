#include "Functions.h"

#if DSP_USE_VC
std::size_t dsp::getNumVectors(std::size_t size) {
    return (size + Vector::Size - 1) / Vector::Size;
}

dsp::Vector dsp::negative(const Vector value) {
    return -value;
}

dsp::Vector dsp::oneOver(const Vector value) {
    return 1.0 / value;
}

dsp::Vector dsp::toBinary(const Vector value) {
    return Vc::iif(value == 0.0, Vector::Zero(), Vector::One());
}

dsp::Vector dsp::toInteger(const Vector value) {
    return Vc::floor(value);
}

dsp::Vector dsp::bipolarToUnipolar(const Vector bipolar) {
    return 0.5 * bipolar + 0.5;
}

dsp::Vector dsp::unipolarToBipolar(const Vector unipolar) {
    return 2.0 * unipolar - 1.0;
}

dsp::Vector dsp::decibelsToLinear(const Vector decibels) {
    return ONE_OVER_SIX_DB * decibels;
}

dsp::Vector dsp::linearToDecibels(const Vector linear) {
    return SIX_DB * linear;
}

dsp::Vector dsp::decibelsToRatio(const Vector decibels) {
    return Vc::exp(LOG2 * decibelsToLinear(decibels));
}

dsp::Vector dsp::ratioToDecibels(const Vector ratio) {
    return linearToDecibels(Vc::log2(ratio));
}

dsp::Vector dsp::clip(const Vector signal, const Vector min, const Vector max) {
    return Vc::iif(min >= max || signal < min, min, Vc::iif(signal > max, max, signal));
}

dsp::Vector dsp::wrap(const Vector signal, const Vector min, const Vector max) {
    const Vector diff = max - min;
    return Vc::iif(diff == 0.0, min, signal - Vc::floor((signal - min) / diff) * diff);
}

std::function<dsp::Sample(dsp::Sample)> dsp::getValue(const Array &table, std::size_t offset) {
    return [offset, &table](Sample index) { return table[(static_cast<std::size_t>(index) + offset) % table.size()]; };
}

dsp::Vector dsp::linear(const Array &table, const Vector index, Vector defaultValue) {
    assert(Vc::all_of(index >= 0.0));
    if (table.size() == 0) {
        return defaultValue;
    }
    Vector indexFloor = Vc::floor(index);
    Vector mu = index - indexFloor;
    Vector x1 = indexFloor.apply(getValue(table, 0));
    Vector x2 = indexFloor.apply(getValue(table, 1));
    return x1 + mu * (x2 - x1);
}

dsp::Vector dsp::hermite(const Array &table, const Vector index, Vector defaultValue) {
    assert(Vc::all_of(index >= 0.0));
    if (table.size() == 0) {
        return defaultValue;
    }
    Vector indexFloor = Vc::floor(index);
    Vector mu = index - indexFloor;
    Vector x0 = indexFloor.apply(getValue(table, table.size() - 1));
    Vector x1 = indexFloor.apply(getValue(table, 0));
    Vector x2 = indexFloor.apply(getValue(table, 1));
    Vector x3 = indexFloor.apply(getValue(table, 2));
    Vector a = 0.5 * (3.0 * (x1 - x2) - x0 + x3);
    Vector b = x2 + x2 + x0 - 0.5 * (5.0 * x1 + x3);
    Vector c = 0.5 * (x2 - x0);
    return ((a * mu + b) * mu + c) * mu + x1;
}
#else
dsp::Sample dsp::negative(const Sample value) {
    return -value;
}

dsp::Sample dsp::oneOver(const Sample value) {
    return 1.0 / value;
}

dsp::Sample dsp::toBinary(const Sample value) {
    return value == 0.0 ? 0.0 : 1.0;
}

dsp::Sample dsp::toInteger(const Sample value) {
    return floor(value);
}

dsp::Sample dsp::bipolarToUnipolar(const Sample bipolar) {
    return 0.5 * bipolar + 0.5;
}

dsp::Sample dsp::unipolarToBipolar(const Sample unipolar) {
    return 2.0 * unipolar - 1.0;
}

dsp::Sample dsp::decibelsToLinear(const Sample decibels) {
    return ONE_OVER_SIX_DB * decibels;
}

dsp::Sample dsp::linearToDecibels(const Sample linear) {
    return SIX_DB * linear;
}

dsp::Sample dsp::decibelsToRatio(const Sample decibels) {
    return exp2(decibelsToLinear(decibels));
}

dsp::Sample dsp::ratioToDecibels(const Sample ratio) {
    return linearToDecibels(log2(ratio));
}
#endif

dsp::Sample dsp::clip(const Sample signal, const Sample min, const Sample max) {
    return (min >= max || signal < min) ? min : signal > max ? max : signal;
}

dsp::Sample dsp::wrap(const Sample signal, const Sample min, const Sample max) {
    if (min == max) {
        return min;
    }
    const Sample diff = max - min;
    return signal - floor((signal - min) / diff) * diff;
}

dsp::Sample dsp::linear(const Array &table, const Sample index, Sample defaultValue) {
    assert(index >= 0.0);
    if (table.size() == 0) {
        return defaultValue;
    }
    int indexFloor = static_cast<int>(index);
    Sample mu = index - indexFloor;
    Sample x1 = table[indexFloor % table.size()];
    Sample x2 = table[(indexFloor + 1) % table.size()];
    return x1 + mu * (x2 - x1);
}

dsp::Sample dsp::hermite(const Array &table, const Sample index, Sample defaultValue) {
    assert(index >= 0.0);
    if (table.size() == 0) {
        return defaultValue;
    }
    int indexFloor = static_cast<int>(index);
    Sample mu = index - indexFloor;
    Sample x0 = table[(indexFloor + table.size() - 1) % table.size()];
    Sample x1 = table[indexFloor % table.size()];
    Sample x2 = table[(indexFloor + 1) % table.size()];
    Sample x3 = table[(indexFloor + 2) % table.size()];
    Sample a = 0.5 * (3.0 * (x1 - x2) - x0 + x3);
    Sample b = x2 + x2 + x0 - 0.5 * (5.0 * x1 + x3);
    Sample c = 0.5 * (x2 - x0);
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

void dsp::ScaledFFT::fft(Array &timeBuffer, Array &realBuffer, Array &imaginaryBuffer) {
    std::transform(timeBuffer.begin(), timeBuffer.end(), time.begin(), [this](Sample x) { return x * oneOverSize; });
    audioFFT.fft(time.data(), real.data(), imaginary.data());
    std::copy(real.begin(), real.end(), realBuffer.begin());
    std::copy(imaginary.begin(), imaginary.end(), imaginaryBuffer.begin());
}

void dsp::ScaledFFT::ifft(Array &timeBuffer, Array &realBuffer, Array &imaginaryBuffer) {
    std::copy(realBuffer.begin(), realBuffer.end(), real.begin());
    std::copy(imaginaryBuffer.begin(), imaginaryBuffer.end(), imaginary.begin());
    audioFFT.ifft(time.data(), real.data(), imaginary.data());
    std::transform(time.begin(), time.end(), timeBuffer.begin(), [this](Sample x) { return x * size; });
}
