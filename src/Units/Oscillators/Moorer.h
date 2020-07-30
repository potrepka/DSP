#pragma once

#include "Generator.h"

namespace dsp {

class Moorer : public Generator {

public:
    enum class Mode { ONE_SIDED, TWO_SIDED };

    Moorer();

    Mode getMode() const;
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getPhase() const;
    std::shared_ptr<InputParameter> getModulationRatio() const;
    std::shared_ptr<InputParameter> getIntensity() const;
    std::shared_ptr<InputParameter> getOvertones() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> phase;
    const std::shared_ptr<InputParameter> modulationRatio;
    const std::shared_ptr<InputParameter> intensity;
    const std::shared_ptr<InputParameter> overtones;

    Mode mode;
};

} // namespace dsp
