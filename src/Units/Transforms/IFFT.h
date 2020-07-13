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
    static const unsigned int REAL;
    static const unsigned int IMAGINARY;

    audiofft::AudioFFT fft;
    std::vector<float> real;
    std::vector<float> imaginary;
    std::vector<float> output;
};

} // namespace dsp
