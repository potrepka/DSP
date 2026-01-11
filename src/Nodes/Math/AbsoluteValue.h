#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class AbsoluteValue : public Transformer {
public:
  AbsoluteValue(Type type = Type::RATIO, Domain domain = Domain::TIME);

protected:
  void processNoLock() override;
};

} // namespace dsp
