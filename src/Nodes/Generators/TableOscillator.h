#pragma once

#include "../Core/Producer.h"

namespace dsp {

class TableOscillator : public Producer {

public:
    TableOscillator(Type type);

    Interpolation getPositionInterpolation() const;
    void setPositionInterpolation(Interpolation interpolation);
    Interpolation getPhaseInterpolation() const;
    void setPhaseInterpolation(Interpolation interpolation);

    std::vector<std::shared_ptr<Buffer>> &getTables();

    std::shared_ptr<Input> getPosition() const;
    std::shared_ptr<Input> getPhase() const;

protected:
    void processNoLock() override;

private:
    Interpolation positionInterpolation;
    Interpolation phaseInterpolation;
    std::vector<std::shared_ptr<Buffer>> tables;
    const std::shared_ptr<Input> position;
    const std::shared_ptr<Input> phase;
};

} // namespace dsp
