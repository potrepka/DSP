#pragma once

#include "Generator.h"

namespace dsp {

class Phasor : public Generator {

public:
    Phasor();

    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getFrequency() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> frequency;

    Array phase;
};

} // namespace dsp
