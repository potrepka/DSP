#pragma once

#include "Processor.h"

namespace dsp {

class HertzToSeconds : public Processor {

public:
    HertzToSeconds(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
