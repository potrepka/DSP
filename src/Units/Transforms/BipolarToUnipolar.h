#pragma once

#include "Processor.h"

namespace dsp {

class BipolarToUnipolar : public Processor {

public:
    BipolarToUnipolar(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
