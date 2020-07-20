#pragma once

#include "Generator.h"

namespace dsp {

class WhiteNoise : public Generator {

public:
    WhiteNoise();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    std::vector<int> seed;
};

} // namespace dsp
