#pragma once

#include "../../Core/Producer.h"

namespace dsp {

class Phasor : public Producer {
public:
  struct Mode {
    static constexpr int MIN = 0;
    static constexpr int MAX = 1;
    static constexpr int WRAPPED = 0;
    static constexpr int UNBOUNDED = 1;
  };

  Phasor();

  std::shared_ptr<Input> getFrequency() const;
  std::shared_ptr<Input> getMode() const;
  std::shared_ptr<Input> getReset() const;

protected:
  void setNumOutputChannelsNoLock(size_t numChannels) override;
  void processNoLock() override;

private:
  const std::shared_ptr<Input> frequency;
  const std::shared_ptr<Input> mode;
  const std::shared_ptr<Input> reset;
  Array phase;
};

} // namespace dsp
