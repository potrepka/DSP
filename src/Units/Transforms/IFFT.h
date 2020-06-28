#pragma once

#include "AudioFFT.h"
#include "Generator.h"

namespace dsp {

class IFFT : public Generator {

public:
    IFFT();
    std::shared_ptr<InputParameter> getReal();
    std::shared_ptr<InputParameter> getImaginary();

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    static const std::size_t REAL;
    static const std::size_t IMAGINARY;
    audiofft::AudioFFT fft;
    std::vector<float> real;
    std::vector<float> imaginary;
    std::vector<float> output;
};

} // namespace dsp
