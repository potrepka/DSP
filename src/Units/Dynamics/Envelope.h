#pragma once

#include "Generator.h"

namespace dsp {

class Envelope : public Generator {

public:
    enum class Mode { LINEAR, EXPONENTIAL };

    Envelope();

    Mode getAttackMode() const;
    void setAttackMode(Mode mode);
    Mode getReleaseMode() const;
    void getReleaseMode(Mode mode);
    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getGate() const;
    std::shared_ptr<InputParameter> getAttack() const;
    std::shared_ptr<InputParameter> getRelease() const;
    std::shared_ptr<OutputParameter> getCurrentTime() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> gate;
    const std::shared_ptr<InputParameter> attack;
    const std::shared_ptr<InputParameter> release;

    const std::shared_ptr<OutputParameter> currentTime;

    Mode attackMode;
    Mode releaseMode;
    std::vector<unsigned int> attackIndex;
    std::vector<unsigned int> releaseIndex;
    Array value;
};

} // namespace dsp
