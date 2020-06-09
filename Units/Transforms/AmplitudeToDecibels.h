#pragma once

#include "Processor.h"

namespace dsp {

class AmplitudeToDecibels : public Processor {

public:
    AmplitudeToDecibels();

protected:
    void process() override;
};

} // namespace dsp
