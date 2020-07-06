#pragma once

#include "Processor.h"

namespace dsp {

class HardClip : public Processor {

public:
    HardClip(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
