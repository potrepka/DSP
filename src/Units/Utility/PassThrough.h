#pragma once

#include "Processor.h"

namespace dsp {

class PassThrough : public Processor {

public:
    PassThrough(Type type, Space space = Space::TIME);

private:
    void process() override;
};

} // namespace dsp
