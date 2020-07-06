#pragma once

#include "Generator.h"

namespace dsp {

class AHR : public Generator {

public:
    AHR();
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getAttack();
    std::shared_ptr<InputParameter> getHold();
    std::shared_ptr<InputParameter> getRelease();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int ATTACK;
    static const unsigned int HOLD;
    static const unsigned int RELEASE;
    std::vector<unsigned int> position;
    std::vector<DSP_FLOAT> value;
};

} // namespace dsp
