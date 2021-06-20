#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Logarithm : public Transformer {

public:
    Logarithm(Space space = Space::TIME);

    std::shared_ptr<Input> getBase() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> base;
};

} // namespace dsp
