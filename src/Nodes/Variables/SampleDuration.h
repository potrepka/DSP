#pragma once

#include "../Core/Producer.h"

namespace dsp {

class SampleDuration : public Producer {

public:
    SampleDuration();

protected:
    void setSampleRateNoLock(double sampleRate) override;
};

} // namespace dsp
