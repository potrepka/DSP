#pragma once

#include "Processor.h"

namespace dsp {

class HardClip : public Processor {

public:
    HardClip();

protected:
    void process() override;
};

} // namespace dsp
