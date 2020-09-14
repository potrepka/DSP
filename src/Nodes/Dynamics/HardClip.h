#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class HardClip : public Transformer {

public:
    HardClip(Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
