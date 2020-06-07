#pragma once

#include "Consumer.h"

namespace dsp {

class SampleRecorder : public Consumer {

public:
    SampleRecorder();
    void setNumChannels(std::size_t size) override;
    std::vector<DSP_FLOAT> getSample(std::size_t index);
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getGate();

protected:
    void process() override;

private:
    static const std::size_t RESET_TRIGGER;
    static const std::size_t GATE;
    std::vector<std::vector<DSP_FLOAT>> samples;
    std::vector<DSP_FLOAT> gatePrevious;

};

}
