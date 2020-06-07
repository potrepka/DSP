#pragma once

#include "Filter.h"

namespace dsp {

class Biquad : public Filter {

public:
    Biquad();
    void setNumChannels(std::size_t size) override;
    std::shared_ptr<InputParameter> getFrequency();
    std::shared_ptr<InputParameter> getQ();
    std::shared_ptr<InputParameter> getGain();

protected:
    DSP_FLOAT a0;
    DSP_FLOAT a1;
    DSP_FLOAT a2;
    DSP_FLOAT b0;
    DSP_FLOAT b1;
    DSP_FLOAT b2;
    void process() override;
    virtual void calculateCoefficients(const DSP_FLOAT &frequency, const DSP_FLOAT &q, const DSP_FLOAT &gain) = 0;

private:
    static const std::size_t FREQUENCY;
    static const std::size_t Q;
    static const std::size_t GAIN;
    std::vector<DSP_FLOAT> x1;
    std::vector<DSP_FLOAT> x2;
    std::vector<DSP_FLOAT> y1;
    std::vector<DSP_FLOAT> y2;
};

} // namespace dsp
