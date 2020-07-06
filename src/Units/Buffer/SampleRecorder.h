#pragma once

#include "Consumer.h"

namespace dsp {

class SampleRecorder : public Consumer {

public:
    SampleRecorder();
    std::vector<DSP_FLOAT> getSample(unsigned int channel);
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getGate();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int GATE;
    std::vector<std::vector<DSP_FLOAT>> samples;
    std::vector<DSP_FLOAT> gatePrevious;
};

} // namespace dsp
