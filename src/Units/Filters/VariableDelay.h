#pragma once

#include "Buffer.h"
#include "Processor.h"

namespace dsp {

class VariableDelay : public Processor {

public:
    VariableDelay(Type type);

    unsigned int getMaxDelayTime() const;
    void setMaxDelayTime(Sample seconds);
    std::shared_ptr<InputParameter> getDelayTime() const;
    std::shared_ptr<InputParameter> getFeedback() const;

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> delayTime;
    const std::shared_ptr<InputParameter> feedback;

    Sample maxDelayTime;
    unsigned int writeIndex;
    std::shared_ptr<Buffer> buffer;

    unsigned int getDelayBufferSize();
};

} // namespace dsp
