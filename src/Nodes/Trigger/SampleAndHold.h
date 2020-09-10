#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class SampleAndHold : public Transformer {

public:
    SampleAndHold(Type type);

    std::shared_ptr<Input> getTrigger() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> trigger;
    Array memory;
};

} // namespace dsp
