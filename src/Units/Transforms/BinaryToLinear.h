#pragma once

#include "Identity.h"

namespace dsp {

class BinaryToLinear : public Identity {

public:
    BinaryToLinear(Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
