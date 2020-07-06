#pragma once

#include "Generator.h"

namespace dsp {

class BufferOscillator : public Generator {

public:
    BufferOscillator();
    std::vector<DSP_FLOAT> &getBuffer(unsigned int channel);
    void setBuffer(unsigned int channel, std::vector<DSP_FLOAT> &buffer);
    void removeBuffer(unsigned int channel);
    std::shared_ptr<InputParameter> getPhase();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int PHASE;
    std::vector<std::vector<DSP_FLOAT> *> buffers;
};

} // namespace dsp
