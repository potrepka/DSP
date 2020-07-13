#pragma once

#include "Generator.h"

namespace dsp {

class Pan : public Generator {

public:
    enum class Mode { CONSTANT_POWER, LINEAR };

    Pan(Space space = Space::TIME);
    Mode getMode() const;
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getDirection() const;

protected:
    void process() override;

private:
    static const unsigned int DIRECTION;
    Mode mode;
};

} // namespace dsp
