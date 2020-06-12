#pragma once

#include "Generator.h"
#include <random>

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
