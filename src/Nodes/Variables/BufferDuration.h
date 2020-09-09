#pragma once

#include "../Core/Producer.h"

namespace dsp {

class BufferDuration : public Producer {

public:
    BufferDuration();

protected:
    void setNumSamplesNoLock(int numSamples) override;
    void setSampleRateNoLock(double sampleRate) override;
};

} // namespace dsp
