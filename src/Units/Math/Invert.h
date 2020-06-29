#pragma once

#include "Processor.h"

namespace dsp {

class Invert : public Processor {

public:
    Invert(Connection::Type type, Connection::Space space = Connection::Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
