#pragma once

#include "Unit.h"

namespace dsp {

class Generator : virtual public Unit {

public:
    Generator(Connection::Type type);
    std::shared_ptr<OutputParameter> getOutputSignal();
    void setOutputSignal(std::shared_ptr<OutputParameter> output);

private:
    static const std::size_t OUTPUT_SIGNAL;
};

} // namespace dsp
