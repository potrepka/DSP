#pragma once

#include "Buffer.h"
#include "Consumer.h"

namespace dsp {

class BufferRecorder : public Consumer {

public:
    enum class Mode { SINGLE_BUFFER, DOUBLE_BUFFER };

    BufferRecorder(Type type, Space space = Space::TIME, Sample defaultValue = 0.0);

    Mode getMode() const;
    void setMode(Mode mode);
    Sample getExternalBufferSize() const;
    void setExternalBufferSize(Sample externalBufferSize);
    bool isExternalBufferSizeSynced() const;
    void setExternalBufferSizeSynced(bool externalBufferSizeSynced);
    std::shared_ptr<Buffer> getBuffer() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    Mode mode;
    Sample externalBufferSize;
    bool externalBufferSizeSynced;
    Sample writeIndex;
    std::shared_ptr<Buffer> buffer;
    std::shared_ptr<Buffer> second;

    void setExternalBufferSizeNoLock(Sample externalBufferSize);
};

} // namespace dsp
