#pragma once

#include "Consumer.h"

namespace dsp {

class ChannelMix : public Consumer {

public:
    ChannelMix(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getMixAmount() const;
    std::shared_ptr<OutputParameter> getMid() const;
    std::shared_ptr<OutputParameter> getSide() const;

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> mixAmount;
    const std::shared_ptr<OutputParameter> mid;
    const std::shared_ptr<OutputParameter> side;

    Array buffer;
};

} // namespace dsp
