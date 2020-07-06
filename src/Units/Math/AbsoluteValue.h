#pragma once

#include "Processor.h"

namespace dsp {

class AbsoluteValue : public Processor {

public:
    AbsoluteValue(Type type, Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
