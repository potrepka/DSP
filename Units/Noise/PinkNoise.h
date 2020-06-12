#pragma once

#include "Generator.h"

namespace dsp {

class PinkNoise : public Generator {

public:
    PinkNoise();
    void setNumChannels(std::size_t numChannels) override;

protected:
    void process() override;

private:
    int seed;
    std::vector<std::vector<DSP_FLOAT>> values;
};

}
