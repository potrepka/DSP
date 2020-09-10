#pragma once

#include "../Core/Producer.h"

namespace dsp {

class PinkNoise : public Producer {

public:
    PinkNoise();

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void setNumSamplesNoLock(size_t numSamples) override;
    void processNoLock() override;

private:
    static Data memoryCoefficientsData;
    static Data noiseCoefficientsData;
    static Wrapper memoryCoefficients;
    static Wrapper noiseCoefficients;
    static const Sample delayedNoiseCoefficient;

    std::vector<int> seed;
    Data whiteData;
    Data memoryData;
    Wrapper white;
    Wrapper memory;
};

} // namespace dsp
