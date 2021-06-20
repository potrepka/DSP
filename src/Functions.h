#pragma once

#include "AudioWrapper.h"
#include "MidiBuffer.h"

#include "../lib/FFTConvolver/AudioFFT.h"

#include <cmath>

namespace dsp {

Sample byteToUnipolar(const int value);
int unipolarToByte(const Sample sample);
Sample shortToBipolar(const int value);
int bipolarToShort(const Sample sample);

Sample clip(const Sample sample, const Sample min, const Sample max);
Sample wrap(const Sample sample, const Sample max);

Sample linear(Sample *data, const size_t size, const Sample index, const Sample defaultValue = 0.0);
Sample hermite(Sample *data, const size_t size, const Sample index, const Sample defaultValue = 0.0);

class ScaledFFT {

public:
    void setup(size_t size);
    size_t getSize();
    size_t getComplexSize();
    void toRealImaginary(Sample *timeChannel, Sample *realChannel, Sample *imaginaryChannel);
    void fromRealImaginary(Sample *realChannel, Sample *imaginaryChannel, Sample *timeChannel);
    void toMagnitudePhase(Sample *timeChannel, Sample *magnitudeChannel, Sample *phaseChannel);
    void fromMagnitudePhase(Sample *magnitudeChannel, Sample *phaseChannel, Sample *timeChannel);

private:
    size_t size;
    Sample oneOverSize;
    audiofft::AudioFFT audioFFT;
    std::vector<audiofft::Sample> time;
    std::vector<audiofft::Sample> real;
    std::vector<audiofft::Sample> imaginary;
};

} // namespace dsp
