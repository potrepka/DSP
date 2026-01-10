#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Shaper : public Transformer {
public:
  enum class Mode { HYPERBOLIC, RATIONAL };

  Shaper(Space space = Space::TIME);

  Mode getMode() const;
  void setMode(Mode mode);

  std::shared_ptr<Input> getDrive() const;

  Sample getOutputSample(size_t channel, Sample input);

protected:
  void processNoLock() override;

private:
  Mode mode;
  const std::shared_ptr<Input> drive;

  static Sample getOutputSample(const Sample& input, const Sample& drive,
                                const Mode mode);
};

} // namespace dsp
