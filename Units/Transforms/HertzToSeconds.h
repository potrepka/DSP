#pragma once

#include "Processor.h"

namespace dsp {

class HertzToSeconds : public Processor {

public:
    HertzToSeconds();

protected:
    void process() override;
};

} // namespace dsp
