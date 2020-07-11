#pragma once

#include "Processor.h"

namespace dsp {

class UnipolarToBipolar : public Processor {

public:
    UnipolarToBipolar(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
