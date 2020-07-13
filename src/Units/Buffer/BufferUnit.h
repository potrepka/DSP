#pragma once

#include "Buffer.h"
#include "Consumer.h"

namespace dsp {

class BufferUnit : public Consumer {

public:
    enum class Mode { SINGLE_BUFFER, DOUBLE_BUFFER };

    BufferUnit(Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);

    Mode getMode() const;
    void setMode(Mode mode);
    unsigned int getExternalBufferSize() const;
    void setExternalBufferSize(unsigned int externalBufferSize);
    std::shared_ptr<Buffer> getBuffer() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    Mode mode;
    unsigned int externalBufferSize;
    std::shared_ptr<Buffer> buffer;
    std::shared_ptr<Buffer> second;
    unsigned int writeIndex;
};

} // namespace dsp
