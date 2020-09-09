#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class PassThrough : public Transformer {

public:
    PassThrough(Type type, Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
