#pragma once

#include "Consumer.h"
#include "Generator.h"

namespace dsp {

class Processor : public Consumer, public Generator {

public:
    Processor(Connection::Type typeIn, Connection::Type typeOut);

protected:
    void process() override;
};

} // namespace dsp
