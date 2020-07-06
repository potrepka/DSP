#pragma once

#include "Unit.h"

namespace dsp {

class Consumer : virtual public Unit {

public:
    Consumer(Connection::Type type, Connection::Space space = Connection::Space::TIME);
    std::shared_ptr<InputParameter> getInputSignal();
    void setInputSignal(std::shared_ptr<InputParameter> input);

private:
    static const unsigned int INPUT_SIGNAL;
};

} // namespace dsp
