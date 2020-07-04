#pragma once

#include "Processor.h"

namespace dsp {

class Base2Exp : public Processor {

public:
    Base2Exp();

protected:
    void process() override;
};

} // namespace dsp
