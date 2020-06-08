#pragma once

#include "Processor.h"

namespace dsp {

class PassUnit : public Processor {

public:
    PassUnit(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
