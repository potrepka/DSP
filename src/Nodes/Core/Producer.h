#pragma once

#include "../../Node.h"

namespace dsp {

class Producer : virtual public Node {

public:
    Producer(Type type, Space space = Space::TIME);

    std::shared_ptr<Output> getOutput() const;
    void setOutput(std::shared_ptr<Output> output);

private:
    std::shared_ptr<Output> output;
};

} // namespace dsp
