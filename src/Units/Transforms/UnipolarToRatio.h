#pragma once

#include "Identity.h"

namespace dsp {

class UnipolarToRatio : public Identity {

public:
    UnipolarToRatio(Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
