#pragma once

#include "Unit.h"

namespace dsp {

class Generator : virtual public Unit {

public:
    Generator(Connection::Type type, Connection::Space space = Connection::Space::TIME);
    std::shared_ptr<OutputParameter> getOutputSignal();
    void setOutputSignal(std::shared_ptr<OutputParameter> output);

private:
    static const unsigned int OUTPUT_SIGNAL;
};

} // namespace dsp
