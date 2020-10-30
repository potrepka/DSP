#pragma once

#include "../Core/Consumer.h"

namespace dsp {

class Crossfader : public Consumer {

public:
    Crossfader(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getPosition() const;
    std::shared_ptr<Output> getA() const;
    std::shared_ptr<Output> getB() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> position;
    const std::shared_ptr<Output> a;
    const std::shared_ptr<Output> b;
};

} // namespace dsp
