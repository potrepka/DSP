#pragma once

#include "Processor.h"

namespace dsp {

class Multiply : public Processor {

public:
    Multiply(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> pushInputBipolar();
    std::shared_ptr<InputParameter> pushInputUnipolar();
    std::shared_ptr<InputParameter> pushInputRatio();
    std::shared_ptr<InputParameter> pushInputInteger();
    std::shared_ptr<InputParameter> pushInputBinary();

protected:
    void process() override;
};

} // namespace dsp
