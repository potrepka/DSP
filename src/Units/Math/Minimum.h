#pragma once

#include "Processor.h"

namespace dsp {

class Minimum : public Processor {

public:
    Minimum(Type type, Space space = Space::TIME);
    void pushInput();

protected:
    void process() override;

private:
    Type type;
    Space space;
};

} // namespace dsp
