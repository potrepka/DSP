#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Identity : public Transformer {

public:
    Identity(Type type = Type::RATIO, Space space = Space::TIME);
    Identity(Type inType, Type outType, Space space = Space::TIME);
    Identity(Type inType, Type outType, Space inSpace, Space outSpace);

protected:
    void processNoLock() override;
};

} // namespace dsp
