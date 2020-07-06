#pragma once

#include "Processor.h"

namespace dsp {

class HardClip : public Processor {

public:
    HardClip(Connection::Space space = Connection::Space::TIME);

protected:
    void process() override;
};

} // namespace dsp
