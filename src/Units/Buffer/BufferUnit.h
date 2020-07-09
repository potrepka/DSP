#pragma once

#include "Buffer.h"
#include "Consumer.h"

namespace dsp {

class BufferUnit : public Consumer {

public:
    BufferUnit();
    std::shared_ptr<Buffer> getBuffer();
    void setBuffer(std::shared_ptr<Buffer> buffer);

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    unsigned int index;
    std::shared_ptr<Buffer> buffer;
};

} // namespace dsp
