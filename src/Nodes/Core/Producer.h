#pragma once

#include "../../Node.h"

namespace dsp {

class Producer : virtual public Node {

public:
    Producer(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Output> getOutput() const;

private:
    const std::shared_ptr<Output> output;
};

} // namespace dsp
