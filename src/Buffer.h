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

    unsigned int getNumChannels();
    void setNumChannels(unsigned int numChannels);
    void setNumChannelsNoLock(unsigned int numChannels);

    unsigned int getBufferSize();
    void setBufferSize(unsigned int bufferSize);
    void setBufferSizeNoLock(unsigned int bufferSize);

    Type getType();
    void setType(Type type);

    Space getSpace();
    void setSpace(Space space);

    DSP_FLOAT getDefaultValue();
    void setDefaultValue(DSP_FLOAT defaultValue);

    std::vector<DSP_FLOAT> &getChannel(unsigned int channel);
    void fillBuffer(DSP_FLOAT value);
    void fillBufferNoLock(DSP_FLOAT value);

private:
    unsigned int bufferSize;
    Type type;
    Space space;
    DSP_FLOAT defaultValue;
    std::vector<std::vector<DSP_FLOAT>> buffers;
};

} // namespace dsp
