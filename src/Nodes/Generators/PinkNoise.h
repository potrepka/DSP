#pragma once

#include "../Core/Producer.h"

namespace dsp {

class PinkNoise : public Producer {

public:
    PinkNoise();

protected:
    void setNumOutputChannelsNoLock(int numChannels) override;
    void setNumSamplesNoLock(int numSamples) override;
    void processNoLock() override;

private:
    static Data memoryCoefficientsData;
    static Data noiseCoefficientsData;
    static Block memoryCoefficients;
    static Block noiseCoefficients;
    static const Sample delayedNoiseCoefficient;

    std::vector<int> seed;
    Data whiteData;
    Data memoryData;
    Block white;
    Block memory;
};

} // namespace dsp
