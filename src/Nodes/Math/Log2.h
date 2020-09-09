#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Log2 : public Transformer {

public:
    Log2(Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
