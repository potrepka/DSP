#pragma once

#include "Processor.h"

namespace dsp {

class DecibelsToAmplitude : public Processor {

public:
    DecibelsToAmplitude();

protected:
    void process() override;
};

} // namespace dsp
