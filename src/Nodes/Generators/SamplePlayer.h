#pragma once

#include "../Core/Producer.h"

namespace dsp {

class SamplePlayer : public Producer {

public:
    SamplePlayer(Type type = Type::RATIO);

    std::vector<std::shared_ptr<Buffer>> &getSamples();

    std::shared_ptr<Input> getSpeed() const;
    std::shared_ptr<Input> getStartTime() const;
    std::shared_ptr<Input> getSampleIndex() const;
    std::shared_ptr<Input> getInterpolation() const;
    std::shared_ptr<Input> getGate() const;
    std::shared_ptr<Input> getReset() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    std::vector<std::shared_ptr<Buffer>> samples;
    const std::shared_ptr<Input> speed;
    const std::shared_ptr<Input> startTime;
    const std::shared_ptr<Input> sampleIndex;
    const std::shared_ptr<Input> interpolation;
    const std::shared_ptr<Input> gate;
    const std::shared_ptr<Input> reset;
    const std::shared_ptr<Output> currentTime;
    Array readIndex;
};

} // namespace dsp
