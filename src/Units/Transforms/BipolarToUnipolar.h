#pragma once

#include "Processor.h"

namespace dsp {

class BipolarToUnipolar : public Processor {

public:
    BipolarToUnipolar();

protected:
    void process() override;
};

} // namespace dsp
