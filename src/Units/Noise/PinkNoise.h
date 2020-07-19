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
    int seed;
    std::vector<std::vector<Sample>> values;
};

} // namespace dsp
