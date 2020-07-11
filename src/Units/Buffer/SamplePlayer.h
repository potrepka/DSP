#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class SamplePlayer : public Generator {

public:
    SamplePlayer(Type type);
    std::shared_ptr<Buffer> getSample() const;
    void setSample(std::shared_ptr<Buffer> sample);
    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getGate() const;
    std::shared_ptr<InputParameter> getStartPosition() const;
    std::shared_ptr<InputParameter> getSpeed() const;

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
