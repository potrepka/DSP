#pragma once

#include "../Core/Producer.h"

namespace dsp {

class ChannelMerger : public Producer {

public:
    ChannelMerger(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getInput(size_t channel) const;

protected:
    void setNumInputChannelsNoLock(size_t numChannels) override;
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;
};

} // namespace dsp
