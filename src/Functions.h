#pragma once

#include "AudioFFT.h"
#include "Constants.h"
#include <cmath>
#include <vector>

namespace dsp {

DSP_FLOAT negative(const DSP_FLOAT value);
DSP_FLOAT oneOver(const DSP_FLOAT value);

DSP_FLOAT toBinary(const DSP_FLOAT value);
DSP_FLOAT toInteger(const DSP_FLOAT value);

DSP_FLOAT bipolarToUnipolar(const DSP_FLOAT bipolar);
DSP_FLOAT unipolarToBipolar(const DSP_FLOAT unipolar);

DSP_FLOAT decibelsToLinear(const DSP_FLOAT decibels);
DSP_FLOAT linearToDecibels(const DSP_FLOAT linear);

DSP_FLOAT decibelsToRatio(const DSP_FLOAT decibels);
DSP_FLOAT ratioToDecibels(const DSP_FLOAT ratio);

DSP_FLOAT clip(const DSP_FLOAT signal, const DSP_FLOAT min, const DSP_FLOAT max);
DSP_FLOAT wrap(const DSP_FLOAT signal, const DSP_FLOAT min, const DSP_FLOAT max);

DSP_FLOAT linear(const std::vector<DSP_FLOAT> &table, const DSP_FLOAT index, DSP_FLOAT defaultValue = 0.0);
DSP_FLOAT hermite(const std::vector<DSP_FLOAT> &table, const DSP_FLOAT index, DSP_FLOAT defaultValue = 0.0);

class ScaledFFT {

public:
    void setup(std::size_t size);
    std::size_t getSize();
    std::size_t getComplexSize();
    void fft(std::vector<DSP_FLOAT> &timeBuffer,
             std::vector<DSP_FLOAT> &realBuffer,
             std::vector<DSP_FLOAT> &imaginaryBuffer);
    void ifft(std::vector<DSP_FLOAT> &timeBuffer,
              std::vector<DSP_FLOAT> &realBuffer,
              std::vector<DSP_FLOAT> &imaginaryBuffer);

private:
    std::size_t size;
    DSP_FLOAT oneOverSize;
    audiofft::AudioFFT audioFFT;
    std::vector<float> time;
    std::vector<float> real;
    std::vector<float> imaginary;
};

} // namespace dsp
