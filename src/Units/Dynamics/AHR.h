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
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> attack;
    const std::shared_ptr<InputParameter> hold;
    const std::shared_ptr<InputParameter> release;

    const std::shared_ptr<OutputParameter> currentTime;

    std::vector<unsigned int> index;
    std::vector<DSP_FLOAT> value;
};

} // namespace dsp
