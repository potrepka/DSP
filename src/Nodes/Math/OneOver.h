#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class OneOver : public Transformer {

public:
    OneOver(Type type, Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
