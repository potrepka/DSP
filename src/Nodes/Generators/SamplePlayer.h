#pragma once

#include "../Core/Producer.h"

namespace dsp {

class SamplePlayer : public Producer {

public:
    SamplePlayer(Type type);

    Interpolation getInterpolation() const;
    void setInterpolation(Interpolation interpolation);

    std::vector<std::shared_ptr<Buffer>> &getSamples();

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getGate() const;
    std::shared_ptr<Input> getSampleIndex() const;
    std::shared_ptr<Input> getSpeed() const;
    std::shared_ptr<Input> getOffsetTime() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(int numChannels) override;
    void processNoLock() override;

private:
    Interpolation interpolation;
    std::vector<std::shared_ptr<Buffer>> samples;
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> gate;
    const std::shared_ptr<Input> sampleIndex;
    const std::shared_ptr<Input> speed;
    const std::shared_ptr<Input> offsetTime;
    const std::shared_ptr<Output> currentTime;
    Array readIndex;
};

} // namespace dsp
