#pragma once

#include "../../Node.h"

namespace dsp {

class Consumer : virtual public Node {

public:
    Consumer(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getInput() const;

private:
    const std::shared_ptr<Input> input;
};

} // namespace dsp
