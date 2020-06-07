#pragma once

#include "Generator.h"

namespace dsp {

class BufferOscillator : public Generator {

public:
    BufferOscillator();
    void setNumChannels(std::size_t size) override;
    std::vector<DSP_FLOAT> &getBuffer(std::size_t index);
    void setBuffer(std::size_t index, std::vector<DSP_FLOAT> &buffer);
    std::shared_ptr<InputParameter> getPhase();

protected:
    void process() override;

private:
    static const std::size_t PHASE;
    std::vector<std::vector<DSP_FLOAT> *> buffers;
};

} // namespace dsp
