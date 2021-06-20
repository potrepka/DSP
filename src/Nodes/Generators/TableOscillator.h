#pragma once

#include "../Core/Producer.h"

namespace dsp {

class TableOscillator : public Producer {

public:
    TableOscillator(Type type = Type::RATIO);

    std::vector<std::shared_ptr<Buffer>> &getTables();

    std::shared_ptr<Input> getPhase() const;
    std::shared_ptr<Input> getPosition() const;
    std::shared_ptr<Input> getPhaseInterpolation() const;
    std::shared_ptr<Input> getPositionInterpolation() const;

protected:
    void processNoLock() override;

private:
    std::vector<std::shared_ptr<Buffer>> tables;
    const std::shared_ptr<Input> phase;
    const std::shared_ptr<Input> position;
    const std::shared_ptr<Input> phaseInterpolation;
    const std::shared_ptr<Input> positionInterpolation;
};

} // namespace dsp
