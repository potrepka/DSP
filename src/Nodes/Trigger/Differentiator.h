#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Differentiator : public Transformer {

public:
    Differentiator(Type type = Type::RATIO);

    std::shared_ptr<Input> getGate() const;
    std::shared_ptr<Input> getReset() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> gate;
    const std::shared_ptr<Input> reset;
    Array state;
    Array memory;
};

} // namespace dsp
