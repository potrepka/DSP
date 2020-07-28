#pragma once

#include "Generator.h"

namespace dsp {

class Phasor : public Generator {

public:
    enum class Mode { WRAPPED, UNBOUNDED };

    Phasor();

    Mode getMode();
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getFrequency() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> frequency;

    Mode mode;
    Array phase;
};

} // namespace dsp
