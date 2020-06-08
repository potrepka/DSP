#pragma once

#include "Generator.h"

namespace dsp {

class Phasor : public Generator {

public:
    Phasor();
    void setNumChannels(std::size_t size);
    std::shared_ptr<InputParameter> getFrequency();
    std::shared_ptr<InputParameter> getResetTrigger();

protected:
    void process() override;

private:
    static const std::size_t FREQUENCY;
    static const std::size_t RESET_TRIGGER;
    std::vector<DSP_FLOAT> phase;

};

}
