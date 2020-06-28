#pragma once

#include "Processor.h"

namespace dsp {

class AbsoluteValue : public Processor {

public:
    AbsoluteValue(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
