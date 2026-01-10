#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class TableShaper : public Transformer {
public:
  TableShaper(Type outputType = Type::RATIO, Space space = Space::TIME);

  std::vector<std::shared_ptr<Buffer>>& getTables();

  Interpolation getInputInterpolation() const;
  void setInputInterpolation(Interpolation interpolation);

  Interpolation getPositionInterpolation() const;
  void setPositionInterpolation(Interpolation interpolation);

  std::shared_ptr<Input> getPosition() const;

protected:
  void processNoLock() override;

private:
  std::vector<std::shared_ptr<Buffer>> tables;
  Interpolation inputInterpolation;
  Interpolation positionInterpolation;
  const std::shared_ptr<Input> position;
};

} // namespace dsp
