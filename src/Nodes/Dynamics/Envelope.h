#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Envelope : public Producer {

public:
    Envelope();

    std::shared_ptr<Input> getAttack() const;
    std::shared_ptr<Input> getRelease() const;
    std::shared_ptr<Input> getAttackShape() const;
    std::shared_ptr<Input> getReleaseShape() const;
    std::shared_ptr<Input> getGate() const;
    std::shared_ptr<Input> getReset() const;
    std::shared_ptr<Output> getCurrentTime() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> attack;
    const std::shared_ptr<Input> release;
    const std::shared_ptr<Input> attackShape;
    const std::shared_ptr<Input> releaseShape;
    const std::shared_ptr<Input> gate;
    const std::shared_ptr<Input> reset;
    const std::shared_ptr<Output> currentTime;
    std::vector<int> attackIndex;
    std::vector<int> releaseIndex;
    Array state;
};

} // namespace dsp
