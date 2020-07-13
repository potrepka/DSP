#pragma once

#include "Unit.h"

namespace dsp {

class Generator : virtual public Unit {

public:
    Generator(Type type, Space space = Space::TIME);

    std::shared_ptr<OutputParameter> getOutputSignal() const;
    void setOutputSignal(std::shared_ptr<OutputParameter> output);

private:
    static const unsigned int OUTPUT_SIGNAL;
};

} // namespace dsp
