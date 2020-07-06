#pragma once

#include "Generator.h"

namespace dsp {

class Phasor : public Generator {

public:
    Phasor();
    std::shared_ptr<InputParameter> getFrequency();
    std::shared_ptr<InputParameter> getResetTrigger();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int FREQUENCY;
    static const unsigned int RESET_TRIGGER;
    std::vector<DSP_FLOAT> phase;
};

} // namespace dsp
