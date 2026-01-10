#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Clipper : public Transformer {
public:
  enum class Mode { CLIP, WRAP, FOLD };

  Clipper(Type type = Type::RATIO, Space space = Space::TIME);

  Mode getMode() const;
  void setMode(Mode mode);

  std::shared_ptr<Input> getMin() const;
  std::shared_ptr<Input> getMax() const;

protected:
  void processNoLock() override;

private:
  Mode mode;
  std::shared_ptr<Input> min;
  std::shared_ptr<Input> max;
};

} // namespace dsp
