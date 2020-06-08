#pragma once

#include "Processor.h"

namespace dsp {

class ToHertz : public Processor {

public:
    ToHertz();

protected:
    void process() override;
};

} // namespace dsp
