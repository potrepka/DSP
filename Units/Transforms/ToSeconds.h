#pragma once

#include "Processor.h"

namespace dsp {

class ToSeconds : public Processor {

public:
    ToSeconds();

protected:
    void process() override;
};

} // namespace dsp
