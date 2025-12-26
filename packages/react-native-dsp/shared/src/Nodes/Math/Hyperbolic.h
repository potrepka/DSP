#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Hyperbolic : public Transformer {
public:
  struct Mode {
    static constexpr int MIN = 0;
    static constexpr int MAX = 2;
    static constexpr int SINE = 0;
    static constexpr int COSINE = 1;
    static constexpr int TANGENT = 2;
  };

  Hyperbolic(Space space = Space::TIME);

  std::shared_ptr<Input> getMode() const;

protected:
  void processNoLock() override;

  const std::shared_ptr<Input> mode;
};

} // namespace dsp
