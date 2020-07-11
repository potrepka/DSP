#pragma once

#include "Generator.h"

namespace dsp {

class SampleRate : public Generator {

public:
    SampleRate(Space space = Space::TIME);

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
};

} // namespace dsp
