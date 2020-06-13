#pragma once

#include "Processor.h"

namespace dsp {

class UnipolarToDecibels : public Processor {

public:
    UnipolarToDecibels();

protected:
    void process() override;
};

} // namespace dsp
