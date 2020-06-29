#pragma once

#include "Processor.h"

namespace dsp {

class Identity : public Processor {

public:
    Identity(Connection::Type inType, Connection::Type outType, Connection::Space space = Connection::Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
