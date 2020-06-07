#pragma once

#include "Biquad.h"

namespace dsp {

class BiquadLowShelf : public Biquad {

protected:
    void calculateCoefficients(const DSP_FLOAT &frequency, const DSP_FLOAT &q, const DSP_FLOAT &gain) override;
};

} // namespace dsp
