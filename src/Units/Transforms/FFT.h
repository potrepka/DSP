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
    const std::shared_ptr<OutputParameter> real;
    const std::shared_ptr<OutputParameter> imaginary;

    audiofft::AudioFFT fft;
    std::vector<float> inputFloats;
    std::vector<float> realFloats;
    std::vector<float> imaginaryFloats;
};

} // namespace dsp
