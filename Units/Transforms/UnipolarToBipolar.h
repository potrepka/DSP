#pragma once

#include "Processor.h"

namespace dsp {

class UnipolarToBipolar : public Processor {

public:
    UnipolarToBipolar();

protected:
    void process() override;
};

} // namespace dsp
