#pragma once

#include "Generator.h"

namespace dsp {

class Counter : Generator {

public:
    Counter();

    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getTrigger() const;
    std::shared_ptr<InputParameter> getOffsetCount() const;
    std::shared_ptr<InputParameter> getSpeed() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> trigger;
    const std::shared_ptr<InputParameter> offset;
    const std::shared_ptr<InputParameter> speed;

    Array memory;
    Array index;
};

} // namespace dsp
