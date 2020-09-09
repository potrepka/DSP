#pragma once

#include "../Core/Producer.h"

namespace dsp {

class FunctionOscillator : public Producer {

public:
    FunctionOscillator(Type type);

    std::function<Sample(Sample)> getFunction();
    void setFunction(std::function<Sample(Sample)> function);

    std::shared_ptr<Input> getPhase();

protected:
    void processNoLock() override;

private:
    std::function<Sample(Sample)> function;
    const std::shared_ptr<Input> phase;
};

} // namespace dsp
