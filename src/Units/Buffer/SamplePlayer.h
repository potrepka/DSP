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
    std::shared_ptr<InputParameter> getOffsetTime() const;
    std::shared_ptr<InputParameter> getSpeed() const;
    std::shared_ptr<OutputParameter> getCurrentTime() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> gate;
    const std::shared_ptr<InputParameter> offsetTime;
    const std::shared_ptr<InputParameter> speed;

    const std::shared_ptr<OutputParameter> currentTime;

    Mode mode;
    std::shared_ptr<Buffer> sample;
    std::vector<DSP_FLOAT> readIndex;
};

} // namespace dsp
