#pragma once

#include "../../Node.h"

namespace dsp {

class Consumer : virtual public Node {

public:
    Consumer(Type type, Space space = Space::TIME);

    std::shared_ptr<Input> getInput() const;
    void setInput(std::shared_ptr<Input> input);

private:
    std::shared_ptr<Input> input;
};

} // namespace dsp
