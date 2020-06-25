#pragma once

#include "Processor.h"

namespace dsp {

class Minimum : public Processor {

public:
    Minimum(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
