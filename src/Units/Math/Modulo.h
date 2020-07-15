#pragma once

#include "Processor.h"

namespace dsp {

class Modulo : public Processor {

public:
    Modulo(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getDivisor() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> divisor;
};

} // namespace dsp
