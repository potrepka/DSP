#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Phasor : public Producer {

public:
    enum class Mode { UNBOUNDED, WRAPPED };

    Phasor();

    Mode getMode() const;
    void setMode(Mode mode);

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getFrequency() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    Mode mode;
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> frequency;
    Array phase;
};

} // namespace dsp
