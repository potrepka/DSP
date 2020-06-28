#pragma once

#include "Processor.h"

namespace dsp {

class RatioToDecibels : public Processor {

public:
    RatioToDecibels();

protected:
    void process() override;
};

} // namespace dsp
