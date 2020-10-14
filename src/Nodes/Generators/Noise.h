#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Noise : public Producer {

public:
    enum class Mode { WHITE, PINK };

    Noise();

    Mode getMode() const;
    void setMode(Mode mode);

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

    Mode mode;
    std::vector<int> seed;
    Data whiteData;
    Data memoryData;
    Wrapper white;
    Wrapper memory;
};

} // namespace dsp
