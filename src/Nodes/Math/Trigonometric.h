#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Trigonometric : public Transformer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int SINE = 0;
        static const int COSINE = 1;
    };

    Trigonometric(Space space = Space::TIME);

    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

    const std::shared_ptr<Input> mode;
};

} // namespace dsp
