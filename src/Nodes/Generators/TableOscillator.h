#pragma once

#include "../Core/Producer.h"

namespace dsp {

class TableOscillator : public Producer {

public:
    TableOscillator(Type type = Type::RATIO);

    Interpolation getPhaseInterpolation() const;
    void setPhaseInterpolation(Interpolation interpolation);
    Interpolation getPositionInterpolation() const;
    void setPositionInterpolation(Interpolation interpolation);

    std::vector<std::shared_ptr<Buffer>> &getTables();

    std::shared_ptr<Input> getPhase() const;
    std::shared_ptr<Input> getPosition() const;

protected:
    void processNoLock() override;

private:
    Interpolation phaseInterpolation;
    Interpolation positionInterpolation;
    std::vector<std::shared_ptr<Buffer>> tables;
    const std::shared_ptr<Input> phase;
    const std::shared_ptr<Input> position;
};

} // namespace dsp
