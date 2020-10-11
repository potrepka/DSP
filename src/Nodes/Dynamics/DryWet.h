#pragma once

#include "../Core/Producer.h"

namespace dsp {

class DryWet : public Producer {

public:
    DryWet(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getDry() const;
    std::shared_ptr<Input> getWet() const;
    std::shared_ptr<Input> getMixAmount() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> dry;
    const std::shared_ptr<Input> wet;
    const std::shared_ptr<Input> mixAmount;
};

} // namespace dsp
