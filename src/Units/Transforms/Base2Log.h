#pragma once

#include "Processor.h"

namespace dsp {

class Base2Log : public Processor {

public:
    Base2Log(Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
