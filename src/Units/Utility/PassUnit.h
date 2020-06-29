#pragma once

#include "Identity.h"

namespace dsp {

class PassUnit : public Identity {

public:
    PassUnit(Connection::Type type, Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
