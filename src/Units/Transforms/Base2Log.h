#pragma once

#include "Processor.h"

namespace dsp {

class Base2Log : public Processor {

public:
    Base2Log();

protected:
    void process() override;
};

} // namespace dsp
