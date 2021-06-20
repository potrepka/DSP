#pragma once

#include "../Core/Consumer.h"

namespace dsp {

class StereoPanner : public Consumer {

public:
    StereoPanner(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getDirection() const;
    std::shared_ptr<Output> getLeft() const;
    std::shared_ptr<Output> getRight() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> direction;
    const std::shared_ptr<Output> left;
    const std::shared_ptr<Output> right;
};

} // namespace dsp
