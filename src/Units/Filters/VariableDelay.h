#pragma once

#include "Processor.h"

namespace dsp {

class VariableDelay : public Processor {

public:
    VariableDelay();
    unsigned int getMaxDelayTime();
    void setMaxDelayTime(DSP_FLOAT seconds);
    std::shared_ptr<InputParameter> getDelayTime();

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int DELAY_TIME;
    DSP_FLOAT maxDelayTime;
    unsigned int writeIndex;
    std::vector<std::vector<DSP_FLOAT>> buffers;
    unsigned int getDelayBufferSize();
    void resizeBuffers();
};

} // namespace dsp
