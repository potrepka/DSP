#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Shaper : public Transformer {

public:
    enum class Mode { POLYNOMIAL, HYPERBOLIC };

    Shaper(Space space = Space::TIME);

    Mode getMode() const;
    void setMode(Mode mode);

    std::shared_ptr<Input> getDrive() const;

protected:
    void processNoLock() override;

private:
    Mode mode;
    const std::shared_ptr<Input> drive;
};

} // namespace dsp
