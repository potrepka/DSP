#pragma once

#include "Processor.h"

namespace dsp {

class DecibelsToRatio : public Processor {

public:
    DecibelsToRatio();

protected:
    void process() override;
};

} // namespace dsp
