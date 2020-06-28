#pragma once

#include "Processor.h"

namespace dsp {

class Multiply : public Processor {

public:
    Multiply(Connection::Type type);
    void pushInput();

protected:
    void process() override;
};

} // namespace dsp
