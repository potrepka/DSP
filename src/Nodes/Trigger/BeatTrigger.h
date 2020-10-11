#pragma once

#include "../Core/Producer.h"

namespace dsp {

class BeatTrigger : public Producer {

public:
    BeatTrigger();

    std::shared_ptr<Input> getIntervalTime() const;
    std::shared_ptr<Input> getDelayTime() const;
    std::shared_ptr<Input> getReset() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> intervalTime;
    const std::shared_ptr<Input> delayTime;
    const std::shared_ptr<Input> reset;
    const std::shared_ptr<Output> currentTime;
    Array index;
};

} // namespace dsp
