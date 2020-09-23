#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class PassThrough : public Transformer {

public:
    PassThrough(Type type, Space space = Space::TIME);
    PassThrough(Type inType, Type outType, Space space = Space::TIME);
    PassThrough(Type inType, Type outType, Space inSpace, Space outSpace);

protected:
    void processNoLock() override;
};

} // namespace dsp
