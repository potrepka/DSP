#pragma once

#include "AudioFFT.h"
#include "Consumer.h"

namespace dsp {

class FFT : public Consumer {

public:
    FFT();
    void setBufferSize(unsigned int bufferSize) override;
    bool getInverted();
    void setInverted(bool inverted);
    std::shared_ptr<OutputParameter> getReal();
    std::shared_ptr<OutputParameter> getImaginary();

protected:
    void process() override;

private:
    static const std::size_t REAL;
    static const std::size_t IMAGINARY;
    audiofft::AudioFFT fft;
    std::vector<float> input;
    std::vector<float> real;
    std::vector<float> imaginary;
    bool inverted;
};

}
