#pragma once

#include "AudioFFT.h"
#include "Consumer.h"

namespace dsp {

class FFT : public Consumer {

public:
    FFT();

    std::shared_ptr<OutputParameter> getReal() const;
    std::shared_ptr<OutputParameter> getImaginary() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    const unsigned int REAL;
    const unsigned int IMAGINARY;

    audiofft::AudioFFT fft;
    std::vector<float> input;
    std::vector<float> real;
    std::vector<float> imaginary;
};

} // namespace dsp
