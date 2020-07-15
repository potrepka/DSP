#pragma once

#include "Processor.h"

namespace dsp {

class DryWetMix : public Processor {

public:
    DryWetMix(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getDrySignal() const;
    std::shared_ptr<InputParameter> getWetSignal() const;
    std::shared_ptr<InputParameter> getMixAmount() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> drySignal;
    const std::shared_ptr<InputParameter> wetSignal;
    const std::shared_ptr<InputParameter> mixAmount;
};

} // namespace dsp
