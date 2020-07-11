#pragma once

#include "Processor.h"

namespace dsp {

class Base2Exp : public Processor {

public:
    Base2Exp(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
