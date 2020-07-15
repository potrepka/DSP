#pragma once

#include "AudioFFT.h"
#include "Generator.h"

namespace dsp {

class IFFT : public Generator {

public:
    IFFT();

    std::shared_ptr<InputParameter> getReal() const;
    std::shared_ptr<InputParameter> getImaginary() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> real;
    const std::shared_ptr<InputParameter> imaginary;

    audiofft::AudioFFT fft;
    std::vector<float> realFloats;
    std::vector<float> imaginaryFloats;
    std::vector<float> outputFloats;
};

} // namespace dsp
