#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Counter : public Producer {

public:
    Counter();

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getTrigger() const;
    std::shared_ptr<Input> getSpeed() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> trigger;
    const std::shared_ptr<Input> speed;
    Array memory;
    Array index;
};

} // namespace dsp
