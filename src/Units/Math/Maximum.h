#pragma once

#include "Processor.h"

namespace dsp {

class Maximum : public Processor {

public:
    Maximum(Type type, Space space = Space::TIME);
    void pushInput();

protected:
    void process() override;

private:
    Type type;
    Space space;
};

} // namespace dsp
