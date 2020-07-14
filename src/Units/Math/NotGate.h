#pragma once

#include "Processor.h"

namespace dsp {

class NotGate : public Processor {

public:
    NotGate(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
