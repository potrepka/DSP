#pragma once

#include "Processor.h"

namespace dsp {

class SampleAndHold : public Processor {

public:
    SampleAndHold(Type type);

    std::shared_ptr<InputParameter> getTrigger() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const unsigned int TRIGGER;

    std::vector<DSP_FLOAT> memory;
};

} // namespace dsp
