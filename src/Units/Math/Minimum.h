#pragma once

#include "Processor.h"

namespace dsp {

class Minimum : public Processor {

public:
    Minimum(Connection::Type type, Connection::Space space = Connection::Space::TIME);
    void pushInput();

protected:
    void process() override;

private:
    Connection::Type type;
    Connection::Space space;
};

} // namespace dsp
