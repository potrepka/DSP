#pragma once

#include "Generator.h"

namespace dsp {

class SampleDuration : public Generator {

public:
    SampleDuration(Space space = Space::TIME);

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
};

} // namespace dsp
