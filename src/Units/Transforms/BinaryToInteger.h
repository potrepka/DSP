#pragma once

#include "Identity.h"

namespace dsp {

class BinaryToInteger : public Identity {

public:
    BinaryToInteger(Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
