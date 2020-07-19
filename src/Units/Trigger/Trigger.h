#pragma once

#include "Generator.h"

namespace dsp {

class Trigger : public Generator {

public:
    Trigger();

    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getIntervalDuration() const;
    std::shared_ptr<InputParameter> getDelayTime() const;
    std::shared_ptr<OutputParameter> getCurrentTime() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> intervalDuration;
    const std::shared_ptr<InputParameter> delayTime;

    const std::shared_ptr<OutputParameter> currentTime;

    Array index;
};

} // namespace dsp
