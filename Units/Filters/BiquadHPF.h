#pragma once

#include "Biquad.h"

namespace dsp {

class BiquadHPF : public Biquad {

public:
    std::shared_ptr<InputParameter> getGain() = delete;

protected:
    void calculateCoefficients(const DSP_FLOAT& frequency, const DSP_FLOAT& q, const DSP_FLOAT& gain) override;

};

}
