#pragma once

#include "Generator.h"

namespace dsp {

class PinkNoise : public Generator {

public:
    PinkNoise();

protected:
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    int seed;
    std::vector<std::vector<DSP_FLOAT>> values;
};

} // namespace dsp
