#pragma once

#include "../Core/Producer.h"

namespace dsp {

class BufferDuration : public Producer {

public:
    BufferDuration();

protected:
    void setNumSamplesNoLock(size_t numSamples) override;
    void setSampleRateNoLock(double sampleRate) override;
};

} // namespace dsp
