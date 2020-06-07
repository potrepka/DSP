#pragma once

#include "Generator.h"

namespace dsp {

class SamplePlayer : Generator {

public:
    SamplePlayer();
    void setNumChannels(std::size_t size) override;
    std::vector<DSP_FLOAT> getSample(std::size_t index);
    void setSample(std::size_t index, const std::vector<DSP_FLOAT> &sample);
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getGate();
    std::shared_ptr<InputParameter> getStartPosition();
    std::shared_ptr<InputParameter> getSpeed();

protected:
    void process() override;

private:
    static const std::size_t RESET_TRIGGER;
    static const std::size_t GATE;
    static const std::size_t START_POSITION;
    static const std::size_t SPEED;
    std::vector<std::vector<DSP_FLOAT>> samples;
    std::vector<DSP_FLOAT> position;
};

} // namespace dsp
