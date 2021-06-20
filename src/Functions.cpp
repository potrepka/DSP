#include "Functions.h"

dsp::Sample dsp::byteToUnipolar(const int value) {
    return value == 0 ? 0.0 : (value + 1) * 0.0078125;
}

int dsp::unipolarToByte(const Sample signal) {
    return signal == 0.0 ? 0 : static_cast<int>(signal * 128) - 1;
}

dsp::Sample dsp::shortToBipolar(const int value) {
    return (value > 8192 ? value - 8191 : value - 8192) * 0.0001220703125;
}

int dsp::bipolarToShort(const Sample signal) {
    return static_cast<int>(signal * 8192) + (signal > 0 ? 8191 : 8192);
}

dsp::Sample dsp::clip(const Sample signal, const Sample min, const Sample max) {
    return max < min || signal < min ? min : signal > max ? max : signal;
}

dsp::Sample dsp::wrap(const Sample signal, const Sample max) {
    return max == 0.0 ? 0.0 : signal - floor(signal / max) * max;
}

dsp::Sample dsp::linear(Sample *data, const size_t length, const Sample index, const Sample defaultValue) {
    DSP_ASSERT(index >= 0.0);
    if (length == 0) {
        return defaultValue;
    }
    size_t indexFloor = static_cast<size_t>(index);
    Sample mu = index - indexFloor;
    Sample x1 = data[indexFloor % length];
    Sample x2 = data[(indexFloor + 1) % length];
    return x1 + mu * (x2 - x1);
}

dsp::Sample dsp::hermite(Sample *data, const size_t length, const Sample index, const Sample defaultValue) {
    DSP_ASSERT(index >= 0.0);
    if (length == 0) {
        return defaultValue;
    }
    size_t indexFloor = static_cast<size_t>(index);
    Sample mu = index - indexFloor;
    Sample x0 = data[(indexFloor + length - 1) % length];
    Sample x1 = data[indexFloor % length];
    Sample x2 = data[(indexFloor + 1) % length];
    Sample x3 = data[(indexFloor + 2) % length];
    Sample a = 0.5 * (3.0 * (x1 - x2) - x0 + x3);
    Sample b = x2 + x2 + x0 - 0.5 * (5.0 * x1 + x3);
    Sample c = 0.5 * (x2 - x0);
    return ((a * mu + b) * mu + c) * mu + x1;
}

void dsp::ScaledFFT::setup(size_t size) {
    this->size = size;
    oneOverSize = 1.0 / size;

    audioFFT.init(size);

    time.resize(size);
    size_t complexSize = getComplexSize();
    real.resize(complexSize);
    imaginary.resize(complexSize);
}

std::size_t dsp::ScaledFFT::getSize() {
    return size;
}

std::size_t dsp::ScaledFFT::getComplexSize() {
    return audiofft::AudioFFT::ComplexSize(size);
}

void dsp::ScaledFFT::toRealImaginary(Sample *timeChannel, Sample *realChannel, Sample *imaginaryChannel) {
    for (size_t k = 0; k < size; ++k) {
        time[k] = timeChannel[k] * oneOverSize;
    }
    audioFFT.fft(time.data(), real.data(), imaginary.data());
    for (size_t k = 0; k < size; ++k) {
        realChannel[k] = real[k];
        imaginaryChannel[k] = imaginary[k];
    }
}

void dsp::ScaledFFT::fromRealImaginary(Sample *realChannel, Sample *imaginaryChannel, Sample *timeChannel) {
    for (size_t k = 0; k < size; ++k) {
        real[k] = realChannel[k];
        imaginary[k] = imaginaryChannel[k];
    }
    audioFFT.ifft(time.data(), real.data(), imaginary.data());
    for (size_t k = 0; k < size; ++k) {
        timeChannel[k] = time[k] * size;
    }
}

void dsp::ScaledFFT::toMagnitudePhase(Sample *timeChannel, Sample *magnitudeChannel, Sample *phaseChannel) {
    for (size_t k = 0; k < size; ++k) {
        time[k] = timeChannel[k] * oneOverSize;
    }
    audioFFT.fft(time.data(), real.data(), imaginary.data());
    for (size_t k = 0; k < getComplexSize(); ++k) {
        magnitudeChannel[k] = sqrt(real[k] * real[k] + imaginary[k] * imaginary[k]);
        Sample bipolar = ONE_OVER_TAU * atan2(imaginary[k], real[k]);
        phaseChannel[k] = bipolar < 0.0 ? bipolar + 1.0 : bipolar;
    }
    for (size_t k = getComplexSize(); k < size; ++k) {
        magnitudeChannel[k] = 0.0;
        phaseChannel[k] = 0.0;
    }
}

void dsp::ScaledFFT::fromMagnitudePhase(Sample *magnitudeChannel, Sample *phaseChannel, Sample *timeChannel) {
    for (size_t k = 0; k < getComplexSize(); ++k) {
        real[k] = magnitudeChannel[k] * cos(TAU * phaseChannel[k]);
        imaginary[k] = magnitudeChannel[k] * sin(TAU * phaseChannel[k]);
    }
    audioFFT.ifft(time.data(), real.data(), imaginary.data());
    for (size_t k = 0; k < size; ++k) {
        timeChannel[k] = time[k] * size;
    }
}
