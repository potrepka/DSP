#pragma once

#include "Processor.h"

namespace dsp {

class Invert : public Processor {

public:
    Invert(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
