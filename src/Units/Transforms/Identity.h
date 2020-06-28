#pragma once

#include "Processor.h"

namespace dsp {

class Identity : public Processor {

public:
    Identity(Connection::Type inType, Connection::Type outType);

protected:
    void process() override;
};

} // namespace dsp
