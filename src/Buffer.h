#pragma once

#include "Constants.h"
#include "Functions.h"
#include "Lockable.h"

namespace dsp {

template <Type T, Space S> class Buffer : public Lockable {

public:
    Buffer(unsigned int size);
    unsigned int getNumChannels();
    void setNumChannels(unsigned int numChannels);
    unsigned int getSize();
    void setSize(unsigned int size);
    std::vector<DSP_FLOAT> &getBuffer(unsigned int channel);

private:
    unsigned int size;
    std::vector<std::vector<DSP_FLOAT>> buffers;
};

} // namespace dsp
