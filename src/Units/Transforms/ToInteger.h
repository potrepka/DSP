#pragma once

#include "Processor.h"

namespace dsp {

class ToInteger : public Processor {

public:
    ToInteger(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
