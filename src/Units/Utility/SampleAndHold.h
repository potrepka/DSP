#pragma once

#include "Processor.h"

namespace dsp {

class SampleAndHold : public Processor {

public:
    SampleAndHold(Connection::Type type);
    std::shared_ptr<InputParameter> getTrigger();

protected:
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t TRIGGER;
    std::vector<DSP_FLOAT> memory;
};

} // namespace dsp
