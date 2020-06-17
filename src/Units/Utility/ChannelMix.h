#pragma once

#include "Processor.h"

namespace dsp {

class ChannelMix : public Processor {

public:
    enum class Mode { MID, SIDE };

    ChannelMix();
    Mode getMode();
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getMix();

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    static const std::size_t MIX;
    Mode mode;
    std::vector<DSP_FLOAT> buffer;
};

} // namespace dsp
