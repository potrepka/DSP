#pragma once

#include "Node.h"

namespace dsp {

class Transformer : public Node {
public:
  Transformer(Type type = Type::RATIO, Domain domain = Domain::TIME);
  Transformer(Type inputType, Type outputType, Domain domain = Domain::TIME);
  Transformer(Type inputType, Type outputType, Domain inDomain,
              Domain outDomain);

  std::shared_ptr<Input> getInput() const;
  std::shared_ptr<Output> getOutput() const;

private:
  const std::shared_ptr<Input> input;
  const std::shared_ptr<Output> output;
};

} // namespace dsp
