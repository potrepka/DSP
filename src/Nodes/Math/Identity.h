#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Identity : public Transformer {
public:
  Identity(Type type = Type::RATIO, Domain domain = Domain::TIME);
  Identity(Type inputType, Type outputType, Domain domain = Domain::TIME);
  Identity(Type inputType, Type outputType, Domain inputDomain,
           Domain outputDomain);

protected:
  void processNoLock() override;
};

} // namespace dsp
