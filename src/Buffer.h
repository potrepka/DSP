#pragma once

#include "Constants.h"
#include "Functions.h"
#include "Lockable.h"

namespace dsp {

class Buffer : public Lockable {

public:
    Buffer(unsigned int numChannels,
           unsigned int bufferSize,
           Type type,
           Space space = Space::TIME,
           DSP_FLOAT defaultValue = 0.0);
    Buffer(const Buffer &buffer);

    unsigned int getNumChannels() const;
    void setNumChannels(unsigned int numChannels);
    void setNumChannelsNoLock(unsigned int numChannels);

    unsigned int getBufferSize() const;
    void setBufferSize(unsigned int bufferSize);
    void setBufferSizeNoLock(unsigned int bufferSize);

    Type getType() const;
    void setType(Type type);

    Space getSpace() const;
    void setSpace(Space space);

    DSP_FLOAT getDefaultValue() const;
    void setDefaultValue(DSP_FLOAT defaultValue);

    std::vector<DSP_FLOAT> &getChannel(unsigned int channel);

    void fillBuffer(DSP_FLOAT value);
    void clearBuffer();

    void clip(unsigned int start, unsigned int end);
    void stretch(unsigned int bufferSize);
    void insert(unsigned int index, std::shared_ptr<Buffer> buffer);

    std::shared_ptr<Buffer> clone();

private:
    unsigned int bufferSize;
    Type type;
    Space space;
    DSP_FLOAT defaultValue;
    std::vector<std::vector<DSP_FLOAT>> buffers;
};

} // namespace dsp
