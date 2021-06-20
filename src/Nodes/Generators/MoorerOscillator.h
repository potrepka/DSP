#pragma once

#include "../Core/Producer.h"

namespace dsp {

class MoorerOscillator : public Producer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int ONE_SIDED = 0;
        static const int TWO_SIDED = 1;
    };

    MoorerOscillator();

    std::shared_ptr<Input> getPhase() const;
    std::shared_ptr<Input> getIntensity() const;
    std::shared_ptr<Input> getModulationIndex() const;
    std::shared_ptr<Input> getHarmonics() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> phase;
    const std::shared_ptr<Input> intensity;
    const std::shared_ptr<Input> modulationIndex;
    const std::shared_ptr<Input> harmonics;
    const std::shared_ptr<Input> mode;
};

} // namespace dsp
