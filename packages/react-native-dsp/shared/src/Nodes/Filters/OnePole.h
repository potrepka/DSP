#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class OnePole : public Transformer {
public:
  struct Mode {
    static constexpr int MIN = 0;
    static constexpr int MAX = 1;
    static constexpr int LOW_PASS = 0;
    static constexpr int HIGH_PASS = 1;
  };

  OnePole(Type type = Type::RATIO);

  std::shared_ptr<Input> getFrequency() const;
  std::shared_ptr<Input> getMode() const;

protected:
  void setNumOutputChannelsNoLock(size_t numChannels) override;
  void processNoLock() override;

private:
  const std::shared_ptr<Input> frequency;
  const std::shared_ptr<Input> mode;
  Array state;
};

} // namespace dsp
