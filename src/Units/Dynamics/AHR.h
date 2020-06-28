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
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t RESET_TRIGGER;
    static const std::size_t ATTACK;
    static const std::size_t HOLD;
    static const std::size_t RELEASE;
    std::vector<unsigned int> position;
    std::vector<DSP_FLOAT> value;
};

} // namespace dsp
