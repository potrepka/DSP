#include "NormalizedFFT.h"

dsp::NormalizedFFT::NormalizedFFT() : size(0), oneOverSize(0.0) {}

void dsp::NormalizedFFT::setup(size_t size) {
  this->size = size;
  oneOverSize = 1.0 / size;

  audioFFT.init(size);

  time.resize(size);
  size_t complexSize = getComplexSize();
  real.resize(complexSize);
  imaginary.resize(complexSize);
}

std::size_t dsp::NormalizedFFT::getSize() { return size; }

std::size_t dsp::NormalizedFFT::getComplexSize() {
  return audiofft::AudioFFT::ComplexSize(size);
}

void dsp::NormalizedFFT::toRealImaginary(Sample* timeChannel,
                                         Sample* realChannel,
                                         Sample* imaginaryChannel) {
  for (size_t k = 0; k < size; ++k) {
    time[k] = timeChannel[k] * oneOverSize;
  }
  audioFFT.fft(time.data(), real.data(), imaginary.data());
  for (size_t k = 0; k < size; ++k) {
    realChannel[k] = real[k];
    imaginaryChannel[k] = imaginary[k];
  }
}

void dsp::NormalizedFFT::fromRealImaginary(Sample* realChannel,
                                           Sample* imaginaryChannel,
                                           Sample* timeChannel) {
  for (size_t k = 0; k < size; ++k) {
    real[k] = realChannel[k];
    imaginary[k] = imaginaryChannel[k];
  }
  audioFFT.ifft(time.data(), real.data(), imaginary.data());
  for (size_t k = 0; k < size; ++k) {
    timeChannel[k] = time[k] * size;
  }
}

void dsp::NormalizedFFT::toMagnitudePhase(Sample* timeChannel,
                                          Sample* magnitudeChannel,
                                          Sample* phaseChannel) {
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

void dsp::NormalizedFFT::fromMagnitudePhase(Sample* magnitudeChannel,
                                            Sample* phaseChannel,
                                            Sample* timeChannel) {
  for (size_t k = 0; k < getComplexSize(); ++k) {
    real[k] = magnitudeChannel[k] * cos(TAU * phaseChannel[k]);
    imaginary[k] = magnitudeChannel[k] * sin(TAU * phaseChannel[k]);
  }
  audioFFT.ifft(time.data(), real.data(), imaginary.data());
  for (size_t k = 0; k < size; ++k) {
    timeChannel[k] = time[k] * size;
  }
}
