#pragma once

#include "Generator.h"

namespace dsp {

class Trigger : public Generator {

public:
    Trigger();
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getInterval();

protected:
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t RESET_TRIGGER;
    static const std::size_t INTERVAL;
    std::vector<DSP_FLOAT> phase;
};

} // namespace dsp
