#pragma once

#include "Generator.h"

namespace dsp {

class Gate : public Generator {

public:
    Gate();
    std::shared_ptr<InputParameter> getOnTrigger();
    std::shared_ptr<InputParameter> getOffTrigger();

protected:
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t ON_TRIGGER;
    static const std::size_t OFF_TRIGGER;
    std::vector<DSP_FLOAT> state;
};

} // namespace dsp
