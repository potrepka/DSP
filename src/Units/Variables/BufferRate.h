#pragma once

#include "Generator.h"

namespace dsp {

class BufferRate : public Generator {

public:
    BufferRate(Space space = Space::TIME);

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setBufferSizeNoLock(unsigned int bufferSize) override;
};

} // namespace dsp
