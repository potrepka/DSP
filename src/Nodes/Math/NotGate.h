#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class NotGate : public Transformer {

public:
    NotGate(Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
