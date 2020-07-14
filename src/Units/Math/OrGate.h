#pragma once

#include "Generator.h"

namespace dsp {

class OrGate : public Generator {

public:
    OrGate(Space space = Space::TIME);

    void pushInput();

protected:
    void process() override;
};

} // namespace dsp
