#pragma once

#include "Consumer.h"

namespace dsp {

class BufferUnit : public Consumer {

public:
    BufferUnit();
    void setNumChannels(std::size_t numChannels) override;
    std::size_t getUnitBufferSize();
    void setUnitBufferSize(std::size_t size);
    std::vector<DSP_FLOAT> &getUnitBuffer(std::size_t index);

protected:
    void process() override;

private:
    std::size_t unitBufferSize;
    std::size_t index;
    std::vector<std::vector<DSP_FLOAT>> buffers;
};

} // namespace dsp
