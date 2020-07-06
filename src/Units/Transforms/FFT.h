#pragma once

#include "AudioFFT.h"
#include "Consumer.h"

namespace dsp {

class FFT : public Consumer {

public:
    FFT();
    std::shared_ptr<OutputParameter> getReal();
    std::shared_ptr<OutputParameter> getImaginary();

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    static const unsigned int REAL;
    static const unsigned int IMAGINARY;
    audiofft::AudioFFT fft;
    std::vector<float> input;
    std::vector<float> real;
    std::vector<float> imaginary;
};

} // namespace dsp
