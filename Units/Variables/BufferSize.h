#pragma once

#include "Generator.h"

namespace dsp {

class BufferSize : public Generator {

public:
    BufferSize();
    void setBufferSize(unsigned int bufferSize);

};

}
