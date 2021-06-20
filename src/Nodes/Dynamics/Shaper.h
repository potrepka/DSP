#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Shaper : public Transformer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int HYPERBOLIC = 0;
        static const int RATIONAL = 1;
    };

    Shaper(Space space = Space::TIME);

    std::shared_ptr<Input> getDrive() const;
    std::shared_ptr<Input> getMode() const;

    Sample getOutputSignal(size_t channel, Sample input);

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> drive;
    const std::shared_ptr<Input> mode;

    static Sample getOutputSignal(const Sample &input, const Sample &drive, const Sample &mode);
};

} // namespace dsp
