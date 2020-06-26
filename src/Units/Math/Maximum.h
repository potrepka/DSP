#pragma once

#include "Processor.h"

namespace dsp {

class Maximum : public Processor {

public:
    Maximum(Connection::Type type);
    void pushInput();

protected:
    void process() override;

private:
    Connection::Type type;
};

} // namespace dsp
