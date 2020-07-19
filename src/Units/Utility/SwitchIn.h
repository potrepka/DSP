#pragma once

#include "Processor.h"

namespace dsp {

class SwitchIn : public Processor {

public:
    SwitchIn(Type type, Space space = Space::TIME, Sample defaultValue = 0.0);

    std::shared_ptr<InputParameter> getInputIndex() const;
    std::shared_ptr<InputParameter> pushInput();

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> inputIndex;
};

} // namespace dsp
