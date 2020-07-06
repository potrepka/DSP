#pragma once

#include "Processor.h"

namespace dsp {

class ToBinary : public Processor {

public:
    ToBinary(Connection::Type type);

protected:
    void process() override;
};

} // namespace dsp
