#pragma once

#include "Buffer.h"
#include "Consumer.h"

namespace dsp {

class BufferUnit : public Consumer {

public:
    BufferUnit(unsigned int bufferSize);
    std::shared_ptr<Buffer> getBuffer();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    unsigned int index;
    std::shared_ptr<Buffer> buffer;
};

} // namespace dsp
