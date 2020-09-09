#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Exp2 : public Transformer {

public:
    Exp2(Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
