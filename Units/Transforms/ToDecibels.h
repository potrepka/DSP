#pragma once

#include "Processor.h"

namespace dsp {

class ToDecibels : public Processor {

public:
    ToDecibels();

protected:
    void process() override;
};

} // namespace dsp
