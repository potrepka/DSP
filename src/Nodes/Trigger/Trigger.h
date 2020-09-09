#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Trigger : public Producer {

public:
    Trigger();

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getIntervalDuration() const;
    std::shared_ptr<Input> getDelayTime() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(int numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> intervalDuration;
    const std::shared_ptr<Input> delayTime;
    const std::shared_ptr<Output> currentTime;
    Array index;
};

} // namespace dsp
