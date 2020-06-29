#pragma once

#include "Consumer.h"
#include "Generator.h"

namespace dsp {

class Processor : public Consumer, public Generator {

public:
    Processor(Connection::Type inType, Connection::Type outType, Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
