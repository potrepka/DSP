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
    std::shared_ptr<Output> getFeedbackSource() const;
    std::shared_ptr<Input> getFeedbackSink() const;

    std::shared_ptr<Node> getFeedbackProcessor() const;

protected:
    void setNumInputChannelsNoLock(size_t numChannels) override;
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void setSampleRateNoLock(double sampleRate) override;
    void processNoLock() override;

private:
    Sample maxDelayTime;
    const std::shared_ptr<Buffer> buffer;
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> delayTime;
    const std::shared_ptr<Input> decayTime;
    const std::shared_ptr<Output> feedbackSource;
    const std::shared_ptr<Input> feedbackSink;
    const std::shared_ptr<Node> feedbackProcessor;
    size_t indexState;

    size_t getDelayBufferSize();
};

} // namespace dsp
