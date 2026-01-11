#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Hyperbolic : public Transformer {
public:
  enum class Mode { SINE, COSINE, TANGENT };

  Hyperbolic(Domain domain = Domain::TIME);

  Mode getMode() const;
  void setMode(Mode mode);

protected:
  void processNoLock() override;

private:
  Mode mode;
};

} // namespace dsp
