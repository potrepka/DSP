#pragma once

#include "../Core/Producer.h"

namespace dsp {

class SampleRate : public Producer {

public:
    SampleRate();

protected:
    void setSampleRateNoLock(double sampleRate) override;
};

} // namespace dsp
