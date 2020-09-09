#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Multiply : public Transformer {

public:
    Multiply(Type type, Space space = Space::TIME);

    std::shared_ptr<Input> getFactor() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> factor;
};

} // namespace dsp
