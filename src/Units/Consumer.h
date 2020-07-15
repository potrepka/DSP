#pragma once

#include "Unit.h"

namespace dsp {

class Consumer : virtual public Unit {

public:
    Consumer(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getInputSignal() const;
    void setInputSignal(std::shared_ptr<InputParameter> input);

private:
    std::shared_ptr<InputParameter> inputSignal;
};

} // namespace dsp
