#pragma once

#include "Consumer.h"
#include "Generator.h"

namespace dsp {

class Filter : public Consumer, public Generator {

public:
    Filter(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
