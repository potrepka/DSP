#pragma once

#include "../Core/Producer.h"

namespace dsp {

class MoorerOscillator : public Producer {

public:
    enum class Mode { ONE_SIDED, TWO_SIDED };

    MoorerOscillator();

    Mode getMode() const;
    void setMode(Mode mode);

    std::shared_ptr<Input> getPhase() const;
    std::shared_ptr<Input> getIntensity() const;
    std::shared_ptr<Input> getModulationIndex() const;
    std::shared_ptr<Input> getHarmonics() const;

protected:
    void processNoLock() override;

private:
    Mode mode;
    const std::shared_ptr<Input> phase;
    const std::shared_ptr<Input> intensity;
    const std::shared_ptr<Input> modulationIndex;
    const std::shared_ptr<Input> harmonics;
};

} // namespace dsp
