#pragma once

#include "Buffer.h"
#include "Processor.h"

namespace dsp {

class VariableDelay : public Processor {

public:
    VariableDelay(Type type);

    unsigned int getMaxDelayTime() const;
    void setMaxDelayTime(DSP_FLOAT seconds);
    std::shared_ptr<InputParameter> getDelayTime() const;

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const unsigned int DELAY_TIME;

    DSP_FLOAT maxDelayTime;
    unsigned int writeIndex;
    std::shared_ptr<Buffer> buffer;

    unsigned int getDelayBufferSize();
};

} // namespace dsp
