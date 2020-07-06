#pragma once

#include "Processor.h"

namespace dsp {

class OneOver : public Processor {

public:
    OneOver(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
