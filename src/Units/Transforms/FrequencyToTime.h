#pragma once

#include "AudioFFT.h"
#include "Generator.h"

namespace dsp {

class FrequencyToTime : public Generator {

public:
    FrequencyToTime(Type type);

    std::shared_ptr<InputParameter> getReal() const;
    std::shared_ptr<InputParameter> getImaginary() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> real;
    const std::shared_ptr<InputParameter> imaginary;

    ScaledFFT fft;
};

} // namespace dsp
