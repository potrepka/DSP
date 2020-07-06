#pragma once

#include "Consumer.h"

namespace dsp {

class BufferUnit : public Consumer {

public:
    BufferUnit();
    unsigned int getUnitBufferSize();
    void setUnitBufferSize(unsigned int size);
    std::vector<DSP_FLOAT> &getUnitBuffer(unsigned int channel);

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    unsigned int unitBufferSize;
    unsigned int index;
    std::vector<std::vector<DSP_FLOAT>> buffers;
};

} // namespace dsp
