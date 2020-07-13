#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class SamplePlayer : public Generator {

public:
    enum class Mode { ONE_SHOT, LOOP };

    SamplePlayer(Type type);

    Mode getMode() const;
    void setMode(Mode mode);
    std::shared_ptr<Buffer> getSample() const;
    void setSample(std::shared_ptr<Buffer> sample);
    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getGate() const;
    std::shared_ptr<InputParameter> getStartTime() const;
    std::shared_ptr<InputParameter> getSpeed() const;
    std::shared_ptr<OutputParameter> getCurrentTime() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const unsigned int RESET_TRIGGER;
    const unsigned int GATE;
    const unsigned int START_TIME;
    const unsigned int SPEED;

    const unsigned int CURRENT_TIME;

    Mode mode;
    std::shared_ptr<Buffer> sample;
    std::vector<DSP_FLOAT> readIndex;
};

} // namespace dsp
