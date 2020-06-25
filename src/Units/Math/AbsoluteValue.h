#pragma once

#include "Processor.h"

namespace dsp {

class AbsoluteValue : public Processor {

public:
    AbsoluteValue();

protected:
    void process() override;
};

} // namespace dsp
