#pragma once

#include "Generator.h"

namespace dsp {

class Trigger : public Generator {

public:
    Trigger();
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getInterval();
    std::shared_ptr<InputParameter> getDelay();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int INTERVAL;
    static const unsigned int DELAY;
    std::vector<DSP_FLOAT> sampleCount;
};

} // namespace dsp
