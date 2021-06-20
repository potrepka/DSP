#pragma once

#include "../Core/Producer.h"

namespace dsp {

class ClockTrigger : public Producer {

public:
    ClockTrigger();

    std::shared_ptr<Input> getInterval() const;
    std::shared_ptr<Input> getDelayTime() const;
    std::shared_ptr<Input> getReset() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> interval;
    const std::shared_ptr<Input> delayTime;
    const std::shared_ptr<Input> reset;
    const std::shared_ptr<Output> currentTime;
    Array index;
};

} // namespace dsp
