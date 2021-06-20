#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class TriggerHold : public Transformer {

public:
    TriggerHold();

    std::shared_ptr<Input> getHoldTime() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> holdTime;
    const std::shared_ptr<Output> currentTime;
    Array index;
};

} // namespace dsp
