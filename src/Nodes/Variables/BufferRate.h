#pragma once

#include "../Core/Producer.h"

namespace dsp {

class BufferRate : public Producer {

public:
    BufferRate();

protected:
    void setNumSamplesNoLock(int numSamples) override;
    void setSampleRateNoLock(double sampleRate) override;
};

} // namespace dsp
