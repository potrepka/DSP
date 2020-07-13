#pragma once

#include "Processor.h"

namespace dsp {

class Multiply : public Processor {

public:
    Multiply(Type type, Space space = Space::TIME);

    void pushInputBipolar();
    void pushInputUnipolar();
    void pushInputRatio();
    void pushInputInteger();
    void pushInputBinary();

protected:
    void process() override;
};

} // namespace dsp
