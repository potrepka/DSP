#pragma once

#include "../Core/Producer.h"

namespace dsp {

class WhiteNoise : public Producer {

public:
    WhiteNoise();

protected:
    void setNumOutputChannelsNoLock(int numChannels) override;
    void processNoLock() override;

private:
    std::vector<int> seed;
};

} // namespace dsp
