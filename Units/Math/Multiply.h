#pragma once

#include "Processor.h"

namespace dsp {

class Multiply : public Processor {

public:
    Multiply(Connection::Type type);

protected:
    void process() override;
};

}
