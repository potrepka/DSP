#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Trigonometric : public Transformer {
public:
  enum class Mode { SINE, COSINE, TANGENT };

  Trigonometric(Domain domain = Domain::TIME);

  Mode getMode() const;
  void setMode(Mode mode);

protected:
  void processNoLock() override;

private:
  Mode mode;
};

} // namespace dsp
