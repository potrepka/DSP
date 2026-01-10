#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Comparison : public Transformer {
public:
  enum class Mode {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL
  };

  Comparison(Type type = Type::RATIO, Space space = Space::TIME);

  Mode getMode() const;
  void setMode(Mode mode);

  std::shared_ptr<Input> getThreshold() const;

protected:
  void processNoLock() override;

private:
  Mode mode;
  const std::shared_ptr<Input> threshold;
};

} // namespace dsp
