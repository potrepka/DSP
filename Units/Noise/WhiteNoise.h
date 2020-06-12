#pragma once

#include "Generator.h"

namespace dsp {

class WhiteNoise : public Generator {

public:
    WhiteNoise();

protected:
    void process() override;

private:
    int seed;
};

}
