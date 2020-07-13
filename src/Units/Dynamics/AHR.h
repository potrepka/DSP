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
    static const unsigned int RESET_TRIGGER;
    static const unsigned int ATTACK;
    static const unsigned int HOLD;
    static const unsigned int RELEASE;

    static const unsigned int CURRENT_TIME;

    std::vector<unsigned int> index;
    std::vector<DSP_FLOAT> value;
};

} // namespace dsp
