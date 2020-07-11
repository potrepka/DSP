#pragma once

#include "Processor.h"

namespace dsp {

class ToBinary : public Processor {

public:
    ToBinary(Type type, Space space = Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
