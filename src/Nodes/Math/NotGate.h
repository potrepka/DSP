#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class NotGate : public Transformer {
public:
  NotGate(Domain domain = Domain::TIME);

protected:
  void processNoLock() override;
};

} // namespace dsp
