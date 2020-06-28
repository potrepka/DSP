#pragma once

#include "Processor.h"

namespace dsp {

class OneOver : public Processor {

public:
    OneOver();

protected:
    void process() override;
};

} // namespace dsp
