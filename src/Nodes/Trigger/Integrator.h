#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Integrator : public Transformer {

public:
    Integrator(Type type);

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getGate() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> gate;
    Array state;
};

} // namespace dsp
