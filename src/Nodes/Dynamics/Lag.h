#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Lag : public Transformer {

public:
    Lag(Type type = Type::RATIO);

    std::shared_ptr<Input> getLagTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> lagTime;
    Array state;
};

} // namespace dsp
