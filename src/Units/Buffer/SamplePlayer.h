#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class SamplePlayer : public Generator {

public:
    SamplePlayer();
    std::shared_ptr<Buffer> getSample();
    void setSample(std::shared_ptr<Buffer> sample);
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
    std::shared_ptr<Buffer> sample;
    std::vector<DSP_FLOAT> readIndex;
};

} // namespace dsp
