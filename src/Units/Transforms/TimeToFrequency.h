#pragma once

#include "AudioFFT.h"
#include "Consumer.h"

namespace dsp {

class TimeToFrequency : public Consumer {

public:
    TimeToFrequency();

    std::shared_ptr<OutputParameter> getReal() const;
    std::shared_ptr<OutputParameter> getImaginary() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    const std::shared_ptr<OutputParameter> real;
    const std::shared_ptr<OutputParameter> imaginary;

    ScaledFFT fft;
};

} // namespace dsp
