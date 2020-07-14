#pragma once

#include "Buffer.h"
#include "Consumer.h"

namespace dsp {

class BufferRecorder : public Consumer {

public:
    enum class Mode { SINGLE_BUFFER, DOUBLE_BUFFER };

    BufferRecorder(Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);

    Mode getMode() const;
    void setMode(Mode mode);
    DSP_FLOAT getExternalBufferSize() const;
    void setExternalBufferSize(DSP_FLOAT externalBufferSize);
    bool isExternalBufferSizeSynced();
    void syncExternalBufferSize();
    void unsyncExternalBufferSize();
    std::shared_ptr<Buffer> getBuffer() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    Mode mode;
    DSP_FLOAT externalBufferSize;
    bool externalBufferSizeSynced;
    DSP_FLOAT writeIndex;
    std::shared_ptr<Buffer> buffer;
    std::shared_ptr<Buffer> second;

    void setExternalBufferSizeNoLock(DSP_FLOAT externalBufferSize);
};

} // namespace dsp
