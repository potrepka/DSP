#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Multiplication : public Transformer {

public:
    Multiplication(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getFactor() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> factor;
};

} // namespace dsp
