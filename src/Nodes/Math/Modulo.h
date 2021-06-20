#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Modulo : public Transformer {

public:
    Modulo(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getDivisor() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> divisor;
};

} // namespace dsp
