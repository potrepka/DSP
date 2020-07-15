#pragma once

#include "Generator.h"

namespace dsp {

class AndGate : public Generator {

public:
    AndGate(Space space = Space::TIME);

    std::shared_ptr<InputParameter> pushInput();

protected:
    void process() override;
};

} // namespace dsp
