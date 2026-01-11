#pragma once

#include "Node.h"

namespace dsp {

class Consumer : public Node {
public:
  Consumer(Type type = Type::RATIO, Domain domain = Domain::TIME);

  std::shared_ptr<Input> getInput() const;

private:
  const std::shared_ptr<Input> input;
};

} // namespace dsp
