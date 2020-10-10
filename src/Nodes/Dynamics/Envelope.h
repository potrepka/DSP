#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Envelope : public Producer {

public:
    enum class Mode { LINEAR, EXPONENTIAL };

    Envelope();

    Mode getAttackMode() const;
    void setAttackMode(Mode mode);
    Mode getReleaseMode() const;
    void getReleaseMode(Mode mode);

    std::shared_ptr<Input> getAttack() const;
    std::shared_ptr<Input> getRelease() const;
    std::shared_ptr<Input> getGate() const;
    std::shared_ptr<Input> getReset() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    Mode attackMode;
    Mode releaseMode;
    const std::shared_ptr<Input> attack;
    const std::shared_ptr<Input> release;
    const std::shared_ptr<Input> gate;
    const std::shared_ptr<Input> reset;
    const std::shared_ptr<Output> currentTime;
    std::vector<int> attackIndex;
    std::vector<int> releaseIndex;
    Array value;
};

} // namespace dsp
