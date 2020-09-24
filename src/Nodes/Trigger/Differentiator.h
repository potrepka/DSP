#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Differentiator : public Transformer {

public:
    Differentiator(Type type);

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getGate() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> gate;
    Array state;
    Array memory;
};

} // namespace dsp
