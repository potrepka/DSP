#pragma once

#include "Buffer.h"
#include "Consumer.h"

namespace dsp {

class SampleRecorder : public Consumer {

public:
    SampleRecorder();
    std::shared_ptr<Buffer> getSample();
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getGate();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int GATE;
    std::shared_ptr<Buffer> sample;
    std::vector<DSP_FLOAT> gatePrevious;
};

} // namespace dsp
