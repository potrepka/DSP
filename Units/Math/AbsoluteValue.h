#pragma once

#include "Processor.h"

namespace dsp {

class AbsoluteValue : public Processor {

public:
    AbsoluteValue();

private:
    void process() override;
};

} // namespace dsp
