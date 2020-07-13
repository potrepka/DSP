#pragma once

#include "Consumer.h"

namespace dsp {

class StereoPanner : public Consumer {

public:
    enum class Mode { CONSTANT_POWER, LINEAR };

    StereoPanner(Type type, Space space = Space::TIME);
    Mode getMode() const;
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getDirection() const;
    std::shared_ptr<OutputParameter> getLeft() const;
    std::shared_ptr<OutputParameter> getRight() const;

protected:
    void process() override;

private:
    static const unsigned int DIRECTION;
    static const unsigned int LEFT;
    static const unsigned int RIGHT;
    Mode mode;
};

} // namespace dsp
