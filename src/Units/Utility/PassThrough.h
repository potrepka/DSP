#pragma once

#include "Identity.h"

namespace dsp {

class PassThrough : public Identity {

public:
    PassThrough(Connection::Type type, Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
