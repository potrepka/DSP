#pragma once

#include "Generator.h"

namespace dsp {

class BufferSize : public Generator {

public:
    BufferSize(Space space = Space::TIME);

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
};

} // namespace dsp
