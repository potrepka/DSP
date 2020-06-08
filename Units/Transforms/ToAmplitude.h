#pragma once

#include "Processor.h"

namespace dsp {

class ToAmplitude : public Processor {

public:
    ToAmplitude();

protected:
    void process() override;
};

} // namespace dsp
