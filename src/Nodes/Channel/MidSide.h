#pragma once

#include "../Core/Consumer.h"

namespace dsp {

class MidSide : public Consumer {

public:
    MidSide(Type type, Space space = Space::TIME);

    std::shared_ptr<Input> getMixAmount() const;
    std::shared_ptr<Output> getMid() const;
    std::shared_ptr<Output> getSide() const;

protected:
    void setNumSamplesNoLock(int numSamples) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> mixAmount;
    const std::shared_ptr<Output> mid;
    const std::shared_ptr<Output> side;
    Data data;
    Block block;
};

} // namespace dsp
