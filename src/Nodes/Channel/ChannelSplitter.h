#pragma once

#include "../Core/Consumer.h"

namespace dsp {

class ChannelSplitter : public Consumer {

public:
    ChannelSplitter(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Output> getOutput(size_t channel) const;

protected:
    void setNumInputChannelsNoLock(size_t numChannels) override;
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;
};

} // namespace dsp
