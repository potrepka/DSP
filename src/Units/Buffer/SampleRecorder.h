#pragma once

#include "Consumer.h"

namespace dsp {

class SampleRecorder : public Consumer {

public:
    SampleRecorder();
    std::vector<DSP_FLOAT> getSample(std::size_t channel);
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getGate();

protected:
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t RESET_TRIGGER;
    static const std::size_t GATE;
    std::vector<std::vector<DSP_FLOAT>> samples;
    std::vector<DSP_FLOAT> gatePrevious;
};

} // namespace dsp
