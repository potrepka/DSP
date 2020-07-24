#pragma once

#include "Generator.h"

namespace dsp {

class FunctionOscillator : public Generator {

public:
    FunctionOscillator(Type type);

    std::function<Sample(Sample)> getFunction();
    void setFunction(std::function<Sample(Sample)> function);
    std::shared_ptr<InputParameter> getPhase();

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> phase;

    std::function<Sample(Sample)> function;
};

} // namespace dsp
