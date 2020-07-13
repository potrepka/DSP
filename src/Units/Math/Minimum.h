#pragma once

#include "Generator.h"

namespace dsp {

class Minimum : public Generator {

public:
    Minimum(Type type, Space space = Space::TIME);

    void pushInput();

protected:
    void process() override;
};

} // namespace dsp
