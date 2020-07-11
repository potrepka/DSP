#pragma once

#include "Processor.h"

namespace dsp {

class ToInteger : public Processor {

public:
    ToInteger(Type type, Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
