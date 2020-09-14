#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class VariableDelay : public Transformer {

public:
    VariableDelay(Type type);

    Sample getMaxDelayTime() const;
    void setMaxDelayTime(Sample seconds);

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getDelayTime() const;
    std::shared_ptr<Input> getDecayTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void setSampleRateNoLock(double sampleRate) override;
    void processNoLock() override;

private:
    Sample maxDelayTime;
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> delayTime;
    const std::shared_ptr<Input> decayTime;
    size_t indexState;
    std::shared_ptr<Buffer> buffer;

    size_t getDelayBufferSize();
};

} // namespace dsp