#pragma once

#include "Generator.h"

namespace dsp {

class PinkNoise : public Generator {

public:
    PinkNoise();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const Sample VALUE_COEFFICIENT_0;
    static const Sample VALUE_COEFFICIENT_1;
    static const Sample VALUE_COEFFICIENT_2;
    static const Sample VALUE_COEFFICIENT_3;
    static const Sample VALUE_COEFFICIENT_4;
    static const Sample VALUE_COEFFICIENT_5;

    static const Sample NOISE_COEFFICIENT_0;
    static const Sample NOISE_COEFFICIENT_1;
    static const Sample NOISE_COEFFICIENT_2;
    static const Sample NOISE_COEFFICIENT_3;
    static const Sample NOISE_COEFFICIENT_4;
    static const Sample NOISE_COEFFICIENT_5;
    static const Sample NOISE_COEFFICIENT_6;
    static const Sample NOISE_COEFFICIENT_7;

    std::vector<int> seed;
#if DSP_USE_VC
    std::vector<Vc::Memory<Vector, 8>> values;
    Vc::Memory<Vector, 8> valueCoefficients;
    Vc::Memory<Vector, 8> noiseCoefficients;
#else
    std::vector<Array> values;
#endif
};

} // namespace dsp
