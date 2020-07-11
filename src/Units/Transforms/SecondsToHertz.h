#pragma once

#include "Processor.h"

namespace dsp {

class SecondsToHertz : public Processor {

public:
    SecondsToHertz(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
