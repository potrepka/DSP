#pragma once

#include "../../Core/Consumer.h"
#include "../../Core/NormalizedFFT.h"

namespace dsp {

class ForwardFFT : public Consumer {
public:
  ForwardFFT();

  std::shared_ptr<Output> getMagnitude() const;
  std::shared_ptr<Output> getPhase() const;

protected:
  void setNumSamplesNoLock(size_t numSamples) override;
  void processNoLock() override;

private:
  const std::shared_ptr<Output> magnitude;
  const std::shared_ptr<Output> phase;

  NormalizedFFT fft;
};

} // namespace dsp
