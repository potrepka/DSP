#pragma once

#include "Processor.h"

namespace dsp {

class Negative : public Processor {

public:
    Negative(Connection::Type type, Connection::Space space = Connection::Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
