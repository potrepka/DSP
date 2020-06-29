#pragma once

#include "Processor.h"

namespace dsp {

class OneOver : public Processor {

public:
    OneOver(Connection::Space space = Connection::Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
