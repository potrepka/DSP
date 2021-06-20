#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Noise : public Producer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int WHITE = 0;
        static const int PINK = 1;
    };

    Noise();

    std::shared_ptr<Input> getMode() const;

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

    const std::shared_ptr<Input> mode;
    std::vector<int> seed;
    Data whiteData;
    Data memoryData;
    Wrapper white;
    Wrapper memory;
};

} // namespace dsp
