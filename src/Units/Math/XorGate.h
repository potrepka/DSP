#pragma once

#include "Generator.h"

namespace dsp {

class XorGate : public Generator {

public:
    XorGate(Space space = Space::TIME);

    unsigned int pushInput();

protected:
    void process() override;
};

} // namespace dsp
