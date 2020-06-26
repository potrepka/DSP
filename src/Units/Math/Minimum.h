#pragma once

#include "Processor.h"

namespace dsp {

class Minimum : public Processor {

public:
    Minimum(Connection::Type type);
    void pushInput();

protected:
    void process() override;

private:
    Connection::Type type;
};

} // namespace dsp
