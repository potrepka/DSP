#pragma once

#include "Processor.h"

namespace dsp {

class AbsoluteValue : public Processor {

public:
    AbsoluteValue(Connection::Type type, Connection::Space space = Connection::Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
