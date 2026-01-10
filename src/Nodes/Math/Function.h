#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Function : public Transformer {
public:
  Function(Type type = Type::RATIO, Space space = Space::TIME);
  Function(Type aType = Type::RATIO, Type bType = Type::RATIO,
           Type outputType = Type::RATIO, Space space = Space::TIME);

  std::shared_ptr<std::function<Sample(Sample, Sample)>> getFunction() const;
  void setFunction(
      std::shared_ptr<std::function<Sample(Sample, Sample)>> function);

  std::shared_ptr<Input> getA() const;
  std::shared_ptr<Input> getB() const;

protected:
  void processNoLock() override;

private:
  std::shared_ptr<std::function<Sample(Sample, Sample)>> function;
  const std::shared_ptr<Input> a;
  const std::shared_ptr<Input> b;
};

} // namespace dsp
