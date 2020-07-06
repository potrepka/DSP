#pragma once

#include "Identity.h"

namespace dsp {

class PassThrough : public Identity {

public:
    PassThrough(Type type, Space space = Space::TIME);
};

} // namespace dsp
