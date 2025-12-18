#pragma once

#include <cmath>
#include <vector>

#include "../lib/FFTConvolver/AudioFFT.h"
#include "Constants.h"

namespace dsp {

class NormalizedFFT {
public:
  NormalizedFFT();

  void setup(size_t size);
  size_t getSize();
  size_t getComplexSize();
  void toRealImaginary(Sample* timeChannel, Sample* realChannel,
                       Sample* imaginaryChannel);
  void fromRealImaginary(Sample* realChannel, Sample* imaginaryChannel,
                         Sample* timeChannel);
  void toMagnitudePhase(Sample* timeChannel, Sample* magnitudeChannel,
                        Sample* phaseChannel);
  void fromMagnitudePhase(Sample* magnitudeChannel, Sample* phaseChannel,
                          Sample* timeChannel);

private:
  size_t size;
  Sample oneOverSize;
  audiofft::AudioFFT audioFFT;
  std::vector<audiofft::Sample> time;
  std::vector<audiofft::Sample> real;
  std::vector<audiofft::Sample> imaginary;
};

} // namespace dsp
