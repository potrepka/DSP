#pragma once

#include "Processor.h"

namespace dsp {

class ChannelMix : public Processor {

public:
    enum class Mode { MID, SIDE };

    ChannelMix(Type type, Space space = Space::TIME);

    Mode getMode() const;
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getMixAmount() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> mixAmount;

    Mode mode;
    Array buffer;
};

} // namespace dsp
