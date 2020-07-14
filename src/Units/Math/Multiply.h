#pragma once

#include "Processor.h"

namespace dsp {

class Multiply : public Processor {

public:
    Multiply(Type type, Space space = Space::TIME);

    unsigned int pushInputBipolar();
    unsigned int pushInputUnipolar();
    unsigned int pushInputRatio();
    unsigned int pushInputInteger();
    unsigned int pushInputBinary();

protected:
    void process() override;
};

} // namespace dsp
