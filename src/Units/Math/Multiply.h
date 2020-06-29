#pragma once

#include "Processor.h"

namespace dsp {

class Multiply : public Processor {

public:
    Multiply(Connection::Type type, Connection::Space space = Connection::Space::TIME);
    void pushInput();

protected:
    void process() override;

private:
    Connection::Space space;
};

} // namespace dsp
