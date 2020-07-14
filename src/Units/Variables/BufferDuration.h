#pragma once

#include "Generator.h"

namespace dsp {

class BufferDuration : public Generator {

public:
    BufferDuration(Space space = Space::TIME);

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setBufferSizeNoLock(unsigned int bufferSize) override;
};

} // namespace dsp
