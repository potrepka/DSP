#pragma once

#include "Generator.h"

namespace dsp {

class AHR : public Generator {

public:
    AHR();

    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getAttack() const;
    std::shared_ptr<InputParameter> getHold() const;
    std::shared_ptr<InputParameter> getRelease() const;
    std::shared_ptr<OutputParameter> getCurrentTime() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const unsigned int RESET_TRIGGER;
    const unsigned int ATTACK;
    const unsigned int HOLD;
    const unsigned int RELEASE;

    const unsigned int CURRENT_TIME;

    std::vector<unsigned int> index;
    std::vector<DSP_FLOAT> value;
};

} // namespace dsp
