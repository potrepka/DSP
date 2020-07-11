#pragma once

#include "Buffer.h"
#include "Consumer.h"

namespace dsp {

class BufferUnit : public Consumer {

public:
    BufferUnit(unsigned int bufferSize);
    std::shared_ptr<Buffer> getBuffer() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    std::shared_ptr<Buffer> buffer;
    unsigned int writeIndex;
};

} // namespace dsp
