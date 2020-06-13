#pragma once

#include "Processor.h"

namespace dsp {

class DecibelsToUnipolar : public Processor {

public:
    DecibelsToUnipolar();

protected:
    void process() override;
};

} // namespace dsp
