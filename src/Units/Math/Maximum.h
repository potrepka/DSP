#pragma once

#include "Processor.h"

namespace dsp {

class Maximum : public Processor {

public:
    Maximum(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
