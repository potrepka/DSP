#pragma once

#include "Generator.h"

namespace dsp {

class Phasor : public Generator {

public:
    Phasor();
    std::shared_ptr<InputParameter> getFrequency();
    std::shared_ptr<InputParameter> getResetTrigger();

protected:
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t FREQUENCY;
    static const std::size_t RESET_TRIGGER;
    std::vector<DSP_FLOAT> phase;
};

} // namespace dsp
