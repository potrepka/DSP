#pragma once

#include "Processor.h"

namespace dsp {

class Negative : public Processor {

public:
    Negative(Type type, Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
