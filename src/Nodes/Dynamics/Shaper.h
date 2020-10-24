#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Shaper : public Transformer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int POLYNOMIAL = 0;
        static const int HYPERBOLIC = 1;
    };

    Shaper(Space space = Space::TIME);

    std::shared_ptr<Input> getDrive() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> drive;
    const std::shared_ptr<Input> mode;
};

} // namespace dsp
