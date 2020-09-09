#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class LogarithmicToRatio : public Transformer {

public:
    LogarithmicToRatio(Space space = Space::TIME);

protected:
    void processNoLock() override;
};

} // namespace dsp
