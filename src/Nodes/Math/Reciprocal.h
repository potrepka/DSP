#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Reciprocal : public Transformer {

public:
    Reciprocal(Type type = Type::RATIO, Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
