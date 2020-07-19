#pragma once

#include "Processor.h"

namespace dsp {

class SwitchOut : public Processor {

public:
    SwitchOut(Type type, Space space = Space::TIME, Sample defaultValue = 0.0);

    std::shared_ptr<InputParameter> getOutputIndex() const;
    std::shared_ptr<OutputParameter> pushOutput();

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> outputIndex;
};

} // namespace dsp
