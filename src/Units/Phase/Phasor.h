#pragma once

#include "Generator.h"

namespace dsp {

class Phasor : public Generator {

public:
    Phasor();
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getFrequency();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int FREQUENCY;
    std::vector<DSP_FLOAT> phase;
};

} // namespace dsp
