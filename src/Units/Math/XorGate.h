#pragma once

#include "Generator.h"

namespace dsp {

class XorGate : public Generator {

public:
    XorGate(Space space = Space::TIME);

    std::shared_ptr<InputParameter> pushInput();

protected:
    void process() override;
};

} // namespace dsp
