#pragma once

#include "Processor.h"

namespace dsp {

class VariableDelay : public Processor {

public:
    VariableDelay();
    std::size_t getMaxDelayTime();
    void setMaxDelayTime(DSP_FLOAT seconds);
    std::shared_ptr<InputParameter> getDelayTime();

protected:
    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t DELAY_TIME;
    DSP_FLOAT maxDelayTime;
    std::size_t writeIndex;
    std::vector<std::vector<DSP_FLOAT>> buffers;
    unsigned int getDelayBufferSize();
    void resizeBuffers();
};

} // namespace dsp
