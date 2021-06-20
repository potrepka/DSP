#pragma once

#include "../Core/Producer.h"

namespace dsp {

class FunctionOscillator : public Producer {

public:
    FunctionOscillator(Type type = Type::RATIO);

    std::shared_ptr<std::function<Sample(Sample)>> getFunction() const;
    void setFunction(std::shared_ptr<std::function<Sample(Sample)>> function);

    std::shared_ptr<Input> getPhase() const;

protected:
    void processNoLock() override;

private:
    std::shared_ptr<std::function<Sample(Sample)>> function;
    const std::shared_ptr<Input> phase;
};

} // namespace dsp
