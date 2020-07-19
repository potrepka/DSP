#pragma once

#include "AudioFFT.h"
#include "Constants.h"
#include <cmath>
#include <vector>

namespace dsp {

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

Sample clip(const Sample signal, const Sample min, const Sample max);
Sample wrap(const Sample signal, const Sample min, const Sample max);

Sample linear(const std::vector<Sample> &table, const Sample index, Sample defaultValue = 0.0);
Sample hermite(const std::vector<Sample> &table, const Sample index, Sample defaultValue = 0.0);

class ScaledFFT {

public:
    void setup(std::size_t size);
    std::size_t getSize();
    std::size_t getComplexSize();
    void fft(std::vector<Sample> &timeBuffer, std::vector<Sample> &realBuffer, std::vector<Sample> &imaginaryBuffer);
    void ifft(std::vector<Sample> &timeBuffer, std::vector<Sample> &realBuffer, std::vector<Sample> &imaginaryBuffer);

private:
    std::size_t size;
    Sample oneOverSize;
    audiofft::AudioFFT audioFFT;
    std::vector<audiofft::Sample> time;
    std::vector<audiofft::Sample> real;
    std::vector<audiofft::Sample> imaginary;
};

} // namespace dsp
