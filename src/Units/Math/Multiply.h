#pragma once

#include "Processor.h"

namespace dsp {

class Multiply : public Processor {

public:
    Multiply(Type type, Space space = Space::TIME);
    void pushInputBipolar();
    void pushInputUnipolar();
    void pushInputRatio();

protected:
    void process() override;

private:
    Type type;
    Space space;
};

} // namespace dsp
