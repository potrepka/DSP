#pragma once

#include "Processor.h"

namespace dsp {

class SecondsToHertz : public Processor {

public:
    SecondsToHertz();

protected:
    void process() override;
};

} // namespace dsp
