#pragma once

#include "Generator.h"

namespace dsp {

class SamplePlayer : public Generator {

public:
    SamplePlayer();
    std::vector<DSP_FLOAT> getSample(unsigned int channel);
    void setSample(unsigned int channel, const std::vector<DSP_FLOAT> &sample);
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getGate();
    std::shared_ptr<InputParameter> getStartPosition();
    std::shared_ptr<InputParameter> getSpeed();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int GATE;
    static const unsigned int START_POSITION;
    static const unsigned int SPEED;
    std::vector<std::vector<DSP_FLOAT>> samples;
    std::vector<DSP_FLOAT> position;
};

} // namespace dsp
