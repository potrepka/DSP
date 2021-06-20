#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Negative : public Transformer {

public:
    Negative(Type type = Type::RATIO, Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
