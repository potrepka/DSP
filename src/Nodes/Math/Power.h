#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Power : public Transformer {

public:
    Power(Space space = Space::TIME);

    std::shared_ptr<Input> getExponent() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> exponent;
};

} // namespace dsp
