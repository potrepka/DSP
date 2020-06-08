#pragma once

#include "Unit.h"

namespace dsp {

class Consumer : virtual public Unit {

public:
    Consumer(Connection::Type type);
    std::shared_ptr<InputParameter> getInputSignal();

protected:
    void process() override;

private:
    static const std::size_t INPUT_SIGNAL;
};

} // namespace dsp
