#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class BooleanMask : public Transformer {

public:
    BooleanMask(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getMask() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> mask;
};

} // namespace dsp
