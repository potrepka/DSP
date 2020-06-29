#pragma once

#include "Identity.h"

namespace dsp {

class BipolarToRatio : public Identity {

public:
    BipolarToRatio(Connection::Space space = Connection::Space::TIME);
};

} // namespace dsp
