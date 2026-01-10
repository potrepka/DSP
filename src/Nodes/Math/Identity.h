#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Identity : public Transformer {
public:
  Identity(Type type = Type::RATIO, Space space = Space::TIME);
  Identity(Type inputType, Type outputType, Space space = Space::TIME);
  Identity(Type inputType, Type outputType, Space inSpace, Space outSpace);

protected:
  void processNoLock() override;
};

} // namespace dsp
