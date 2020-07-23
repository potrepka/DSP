#pragma once

#include "AudioFFT.h"
#include "Constants.h"
#include <cmath>
#include <vector>

namespace dsp {

#if DSP_USE_VC
Vector negative(const Vector value);
Vector oneOver(const Vector value);

Vector toBinary(const Vector value);
Vector toInteger(const Vector value);

Vector bipolarToUnipolar(const Vector bipolar);
Vector unipolarToBipolar(const Vector unipolar);

Vector decibelsToLinear(const Vector decibels);
Vector linearToDecibels(const Vector linear);

Vector decibelsToRatio(const Vector decibels);
Vector ratioToDecibels(const Vector ratio);

Vector clip(Vector signal, const Vector min, const Vector max);
Vector wrap(Vector signal, const Vector max);

std::function<Sample(Sample)> getValue(const Array &table, std::size_t offset);

Vector linear(const Array &table, const Vector index, Vector defaultValue = Vector::Zero());
Vector hermite(const Array &table, const Vector index, Vector defaultValue = Vector::Zero());
#else
Sample negative(const Sample value);
Sample oneOver(const Sample value);

Sample toBinary(const Sample value);
Sample toInteger(const Sample value);

Sample bipolarToUnipolar(const Sample bipolar);
Sample unipolarToBipolar(const Sample unipolar);

Sample decibelsToLinear(const Sample decibels);
Sample linearToDecibels(const Sample linear);

Sample decibelsToRatio(const Sample decibels);
Sample ratioToDecibels(const Sample ratio);
#endif

Sample clip(Sample signal, const Sample min, const Sample max);
Sample wrap(Sample signal, const Sample max);

Sample linear(const Array &table, const Sample index, Sample defaultValue = 0.0);
Sample hermite(const Array &table, const Sample index, Sample defaultValue = 0.0);

class ScaledFFT {

public:
    void setup(std::size_t size);
    std::size_t getSize();
    std::size_t getComplexSize();
    void fft(Array &timeBuffer, Array &realBuffer, Array &imaginaryBuffer);
    void ifft(Array &timeBuffer, Array &realBuffer, Array &imaginaryBuffer);

private:
    std::size_t size;
    Sample oneOverSize;
    audiofft::AudioFFT audioFFT;
    std::vector<audiofft::Sample> time;
    std::vector<audiofft::Sample> real;
    std::vector<audiofft::Sample> imaginary;
};

} // namespace dsp
