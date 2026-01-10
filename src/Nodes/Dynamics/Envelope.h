#pragma once

#include "../../Core/Producer.h"

namespace dsp {

class Envelope : public Producer {
public:
  Envelope();

  Shape getAttackShape() const;
  void setAttackShape(Shape shape);

  Shape getReleaseShape() const;
  void setReleaseShape(Shape shape);

  std::shared_ptr<Input> getAttack() const;
  std::shared_ptr<Input> getRelease() const;
  std::shared_ptr<Input> getGate() const;
  std::shared_ptr<Input> getReset() const;
  std::shared_ptr<Output> getCurrentTime() const;

protected:
  void setNumOutputChannelsNoLock(size_t numChannels) override;
  void processNoLock() override;

private:
  Shape attackShape;
  Shape releaseShape;
  const std::shared_ptr<Input> attack;
  const std::shared_ptr<Input> release;
  const std::shared_ptr<Input> gate;
  const std::shared_ptr<Input> reset;
  const std::shared_ptr<Output> currentTime;
  std::vector<int> attackIndex;
  std::vector<int> releaseIndex;
  Array state;
};

} // namespace dsp
